#include <iostream>
#include <fstream>

#include <stdexcept>

#include "glRenderer.h"

GlRenderer::GlRenderer()
	: mv(glm::mat4(1.0)), mvp(glm::mat4(1.0)), model(glm::mat4(1.0)), view(glm::mat4(1.0)), projection(glm::mat4(1.0)), deltaTime(0), lastFrame(0)
{
	//create hidden window that shares resources with user windows
	try
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		sharedResources = glfwCreateWindow(1, 1, "", NULL, NULL);

		if (sharedResources == NULL)
			throw std::runtime_error("[OpenGL] FATAL: Failed to create GLFW window!\n");

		glfwMakeContextCurrent(sharedResources);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("[OpenGL] FATAL: Failed to initialize GLAD!\n");	
	}
	catch (const std::runtime_error& err)
	{
		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << err.what();
		log << err.what();

		log.close();
		abort();
	}

	//initialize resources using the hidden window's context
	glEnable(GL_DEPTH_TEST);

	vertexBuffer = new GlVertexBuffer(vertices, sizeof(vertices));

	vertexLayout = new GlVertexBufferLayout();
	vertexLayout->push<float>(3); //vertex position
	//vertexLayout->push<float>(2); //texture coordinate
	vertexLayout->push<float>(3); //normal vector direction

	lightVertexLayout = new GlVertexBufferLayout();
	lightVertexLayout->push<float>(3);
	lightVertexLayout->push<float>(2);

	vertexBuffer->unbind();

	lightCubeShader = new GlShader("shaders/shader.vert", "shaders/lightCubeFrag.frag");
	lightingShader = new GlShader("shaders/shader.vert", "shaders/shader.frag");
	texture = new GlTexture("textures/container.jpg", GL_REPEAT, GL_LINEAR);
	texture2 = new GlTexture("textures/awesomeface.png", GL_REPEAT, GL_LINEAR);

	glfwMakeContextCurrent(NULL);
}

GlRenderer::~GlRenderer()
{
	glfwMakeContextCurrent(NULL);

	for(GlVertexArray* vertexArray : vertexArrays)
		vertexArray->~GlVertexArray();

	vertexBuffer->~GlVertexBuffer();

	for (GlWindow* window : windows)
		window->~GlWindow();
}

double GlRenderer::getDeltaTime()
{
	return deltaTime;
}

void GlRenderer::render()
{
	calculateDeltaTime();

	for(uint32_t i = 0; i < windows.size(); i++)
	{
		const auto& window = windows[i];
		const auto& vertexArray = vertexArrays[i];
		const auto& lightVertexArray = lightVertexArrays[i];

		glfwMakeContextCurrent(window->glfwWindow);

		window->processInput(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture->bind(0);
		texture2->bind(1);

		lightingShader->use();
		//lightingShader->setInt("texture1", 0);
		//lightingShader->setInt("texture2", 1);

		lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader->setVec3("lightPos", glm::vec3(10 * lightPos.x * sin(glfwGetTime()), lightPos.y, 10 * lightPos.z * cos(glfwGetTime())));

		view = window->camera->getViewMatrix();
		projection = window->camera->getProjectionMatrix();

		lightingShader->setMat4("view", view);

		vertexArray->bind();

		for (uint32_t j = 0; j < 10; j++)
		{
		    model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[j]);

			float angle = 20.0f * j;

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			mv = view * model;
			mvp = projection * mv;

			lightingShader->setMat4("model", model);
			lightingShader->setMat4("mv", mv);
			lightingShader->setMat4("mvp", mvp);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		vertexArray->unbind();

		//light cube
		lightCubeShader->use();

		lightVertexArray->bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10 * lightPos.x * sin(glfwGetTime()), lightPos.y, 10 * lightPos.z * cos(glfwGetTime())));

		mvp = projection * view * model;
		
		lightCubeShader->setMat4("mvp", mvp);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightVertexArray->unbind();

		glfwSwapBuffers(window->glfwWindow);

		glfwPollEvents();
	}
}

void GlRenderer::addWindow(GlWindow* window)
{
	glfwMakeContextCurrent(window->glfwWindow);

	glEnable(GL_DEPTH_TEST);

	//we need to create a new vertex array for every single window that we add
	vertexArrays.push_back(new GlVertexArray());
	vertexArrays[vertexArrays.size() - 1]->addBuffer(*vertexBuffer, *vertexLayout);

	lightVertexArrays.push_back(new GlVertexArray());
	lightVertexArrays[lightVertexArrays.size() - 1]->addBuffer(*vertexBuffer, *vertexLayout);

	windows.push_back(window);

	glfwMakeContextCurrent(NULL);
}

void GlRenderer::calculateDeltaTime()
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}