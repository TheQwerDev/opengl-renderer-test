#include <iostream>
#include <fstream>

#include <stdexcept>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glWindow.h"

bool GlWindow::shouldClose()
{
	return glfwWindowShouldClose(glfwWindow);
}

void GlWindow::setResolution(const uint32_t& width, const uint32_t& height)
{
	GlWindow::width = width;
	GlWindow::height = height;
	glfwSetWindowSize(glfwWindow, width, height);
}

void GlWindow::setWidth(const uint32_t& width)
{
	GlWindow::width = width;
	glfwSetWindowSize(glfwWindow, width, GlWindow::height);
}

void GlWindow::setHeight(const uint32_t& height)
{
	GlWindow::height = height;
	glfwSetWindowSize(glfwWindow, GlWindow::width, height);
}

void GlWindow::processInput(double deltaTime)
{
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->processKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->processKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera->processKeyboard(DOWN, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->processKeyboard(UP, deltaTime);
}

GlWindow::GlWindow(const uint32_t& width, const uint32_t& height, const char* name, GlCamera* camera, GLFWwindow* rendererSharedResources, bool wireframeMode)
	: width(width), height(height), camera(camera), wireframeMode(wireframeMode), 
	lastMousePosX((float)width / 2), lastMousePosY((float)height / 2), firstMouseMovement(true)
{
	try
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindow = glfwCreateWindow(width, height, name, NULL, rendererSharedResources);

		if (glfwWindow == NULL)
			throw std::runtime_error("[OpenGL] FATAL: Failed to create GLFW window!\n");

		glfwMakeContextCurrent(glfwWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("[OpenGL] FATAL: Failed to initialize GLAD!\n");

		if (GlWindow::wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GlWindow::camera->aspectRatio = (float)GlWindow::width / (float)GlWindow::height;

		glfwSetWindowUserPointer(glfwWindow, this);

		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);
		glfwSetCursorPosCallback(glfwWindow, mouseCallback);
		glfwSetCursorEnterCallback(glfwWindow, cursorEnterCallback);
		glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
		glfwSetScrollCallback(glfwWindow, mouseScrollCallback);

		glfwSetKeyCallback(glfwWindow, keyCallback);

		glViewport(0, 0, width, height); //forcefully set window viewport to prevent weird viewport sizes when creating multiple windows
		glfwSwapInterval(0); //MAKE ONLY THE FIRST WINDOW HAVE V-SYNC IN THE RENDERER CLASS

		glfwMakeContextCurrent(NULL);
	}
	catch (const std::runtime_error& err)
	{
		std::ofstream log("log.txt", std::ios_base::app);

		std::cout << err.what();
		log << err.what();

		log.close();
		abort();
	}
}

GlWindow::~GlWindow()
{
	glfwDestroyWindow(glfwWindow);
}


//callbacks based on the information presented here: https://discourse.glfw.org/t/passing-parameters-to-callbacks/848

void GlWindow::framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	glfwMakeContextCurrent(glWindowPtr->glfwWindow);

	glViewport(0, 0, width, height);
	glWindowPtr->width = width;
	glWindowPtr->height = height;

	glWindowPtr->camera->aspectRatio = (float)width / (float)height;
}

void GlWindow::mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	float xPos = (float)xPosIn;
	float yPos = (float)yPosIn;

	if (glWindowPtr->firstMouseMovement)
	{
		glWindowPtr->lastMousePosX = xPos;
		glWindowPtr->lastMousePosY = yPos;
		glWindowPtr->firstMouseMovement = false;
	}

	float xOffset = xPos - glWindowPtr->lastMousePosX;
	float yOffset = glWindowPtr->lastMousePosY - yPos;

	glWindowPtr->lastMousePosX = xPos;
	glWindowPtr->lastMousePosY = yPos;

	glWindowPtr->camera->processMouse(xOffset, yOffset);
}

void GlWindow::cursorEnterCallback(GLFWwindow* window, int32_t entered)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	if (!entered)
	{
		glWindowPtr->firstMouseMovement = true;

		glWindowPtr->lastMousePosX = (float)glWindowPtr->width / 2;
		glWindowPtr->lastMousePosY = (float)glWindowPtr->height / 2;
	}
}

void GlWindow::mouseButtonCallback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		glfwSetInputMode(glWindowPtr->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void GlWindow::mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	glWindowPtr->camera->processMouseScroll((float)yOffset);
}

void GlWindow::keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	GlWindow* glWindowPtr = (GlWindow*)glfwGetWindowUserPointer(window);
	
	glfwMakeContextCurrent(glWindowPtr->glfwWindow);

	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetInputMode(glWindowPtr->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glWindowPtr->firstMouseMovement = true;

		glWindowPtr->lastMousePosX = (float)glWindowPtr->width / 2;
		glWindowPtr->lastMousePosY = (float)glWindowPtr->height / 2;
	}

	if (key == GLFW_KEY_ESCAPE && mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindowPtr->glfwWindow, true);
	}

	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	{
		if (glWindowPtr->wireframeMode)
		{
			glWindowPtr->wireframeMode = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glWindowPtr->wireframeMode = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}
