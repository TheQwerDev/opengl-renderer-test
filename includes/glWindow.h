#pragma once

#include "glCamera.h"

class GlWindow {
public:
	bool shouldClose();

	void setResolution(const uint32_t& width, const uint32_t& height);
	void setWidth(const uint32_t& width);
	void setHeight(const uint32_t& height);

	void processInput(double deltaTime);

	GlWindow(const uint32_t& width, const uint32_t& height, const char* name, GlCamera* camera, GLFWwindow* rendererSharedResources, bool wireframeMode = false);
	~GlWindow();

	uint32_t width;
    uint32_t height;

	GlCamera* camera;
	GLFWwindow* glfwWindow;

	bool wireframeMode;

private:
	static void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);
	static void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
	static void cursorEnterCallback(GLFWwindow* window, int32_t entered);
	static void mouseButtonCallback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
	static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

	bool firstMouseMovement;

	float lastMousePosX;
	float lastMousePosY;
};