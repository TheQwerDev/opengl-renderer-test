#include <iostream>

#include "glRenderer.h"

int main()
{
	glfwInit();

	GlRenderer renderer;

	GlCamera mainCamera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), PERSPECTIVE);
	GlCamera secondCamera(glm::vec3(3, 0, 0), glm::vec3(0, 1, 0), PERSPECTIVE);
	GlCamera thirdCamera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), PERSPECTIVE);
	GlCamera fourthCamera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), ORTHOGRAPHIC);

	mainCamera.farClip = 1000.0f;
	mainCamera.moveSpeed = 2.5f;
	mainCamera.constrainEulerAngle = 80.0f;

	secondCamera.fieldOfView = 270;

	GlWindow window1(800, 600, "Main window", &mainCamera, renderer.sharedResources);
	renderer.addWindow(&window1);

	//GlWindow window2(300, 768, "Cooler window", &secondCamera, renderer.sharedResources);
	//renderer.addWindow(&window2);
	
	//GlWindow window3(300, 200, "Tiny window", &thirdCamera, renderer.sharedResources);
	//renderer.addWindow(&window3);

	//GlWindow window4(420, 240, "Wacky window", &fourthCamera, renderer.sharedResources);
	//renderer.addWindow(&window4);

	while(!window1.shouldClose())
		renderer.render();

	renderer.~GlRenderer();

	std::cout << "Press any key to continue...\n";

	std::cin.get();

	glfwTerminate();

	return 0;
}