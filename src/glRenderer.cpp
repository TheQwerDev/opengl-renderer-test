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

	lightCubeShader = new GlShader("shaders/shader.vert", "shaders/lightCubeFrag.frag");
	lightingShader = new GlShader("shaders/shader.vert", "shaders/shader.frag");

	glfwMakeContextCurrent(NULL);
}

GlRenderer::~GlRenderer()
{
	glfwMakeContextCurrent(NULL);

	for (GlWindow* window : windows)
		window->~GlWindow();
}

double GlRenderer::getDeltaTime()
{
	return deltaTime;
}

void GlRenderer::render(std::vector<AtlasGameObj*> gameObjects)
{
	calculateDeltaTime();

	for(uint32_t i = 0; i < windows.size(); i++)
	{
		const auto& window = windows[i];

		glfwMakeContextCurrent(window->glfwWindow);

		window->processInput(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = window->camera->getViewMatrix();
		projection = window->camera->getProjectionMatrix();

		lightingShader->use();

		// directional light
		lightingShader->setVec3("dirLight.direction", view * glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f));
		lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// spotLight
		lightingShader->setVec3("spotLight.position", view * glm::vec4(window->camera->position, 1.0f));
		lightingShader->setVec3("spotLight.direction", view * glm::vec4(window->camera->front, 0.0f));
		lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("spotLight.constant", 1.0f);
		lightingShader->setFloat("spotLight.linear", 0.09f);
		lightingShader->setFloat("spotLight.quadratic", 0.032f);
		lightingShader->setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
		lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		lightingShader->setFloat("material.shininess", 32.0f);

		for (AtlasGameObj* gameObject : gameObjects)
		{
			model = glm::mat4(1.0);
			model = glm::translate(model, gameObject->position);

			model = glm::rotate(model, glm::radians(gameObject->rotation.z), glm::vec3(0.0, 0.0, 1.0));
			model = glm::rotate(model, glm::radians(gameObject->rotation.x), glm::vec3(1.0, 0.0, 0.0));
			model = glm::rotate(model, glm::radians(gameObject->rotation.y), glm::vec3(0.0, 1.0, 0.0));

			model = glm::scale(model, gameObject->scale);

			mv = view * model;
			mvp = projection * mv;

			lightingShader->setMat4("model", model);
			lightingShader->setMat4("mv", mv);
			lightingShader->setMat4("mvp", mvp);

			gameObject->model->draw(lightingShader, i);
		}

		glfwSwapBuffers(window->glfwWindow);

		glfwPollEvents();
	}
}

void GlRenderer::addWindow(GlWindow* window, std::vector<AtlasGameObj*> gameObjects)
{
	glfwMakeContextCurrent(window->glfwWindow);

	glEnable(GL_DEPTH_TEST);

	//we need to create new vertex arrays for every single window that we add
	for (GlModel* model : models)
	{
		model->createVertexArrays();
	}

	windows.push_back(window);

	glfwMakeContextCurrent(NULL);
}

void GlRenderer::addModel(GlModel* model)
{
	//create vertex arrays for the model in every single window context
	for (GlWindow* window : windows)
	{
		glfwMakeContextCurrent(window->glfwWindow);
		model->createVertexArrays();
	}
	
	glfwMakeContextCurrent(NULL);

	models.push_back(model);
}

void GlRenderer::calculateDeltaTime()
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}