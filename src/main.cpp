#include <iostream>

#include "atlasInstance.h"

int main()
{
	AtlasInstance instance;

	GlCamera mainCamera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), PERSPECTIVE);
	GlCamera secondCamera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), PERSPECTIVE);

	mainCamera.farClip = 1000.0f;
	mainCamera.moveSpeed = 2.5f;
	mainCamera.constrainEulerAngle = 80.0f;

	//method 1 for adding windows to the game instance
	GlWindow* window1 = instance.newWindow(800, 600, "Main window", &mainCamera);

	//method 2 for adding windows to the instance
	//GlWindow window2(800, 600, "Second window", &secondCamera, instance.getRendererResourcesPtr());
	//instance.addWindow(&window2);

	//method 1 for adding models to the instance
	GlModel* model1 = instance.newModel("models/plane/plane.obj");

	//method 2 for adding models to the instance
	GlModel model2("models/backpack/backpack.obj", instance.getRendererResourcesPtr());
	instance.addModel(&model2); 

	//method 1 for adding game objects to the instance
	AtlasGameObj* gameObject = instance.newGameObj(glm::vec3(4.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(-45, -90, 0), model1);
	AtlasGameObj* gameObject2 = instance.newGameObj(glm::vec3(-4.0, 0.0, 0.0), glm::vec3(0.5), glm::vec3(45, -90, 0), model1);

	//method 2 for adding game objects to the instance
	AtlasGameObj gameObject3(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 0.0), &model2);
	instance.addGameObj(&gameObject3);

	gameObject2->model = &model2;
	gameObject2->setScale(0.5, 0.5, 0.5);
	gameObject2->setRotation(-45, -45, -45);

	while (!window1->shouldClose())
		instance.update();

	instance.~AtlasInstance();

	std::cout << "Press any key to continue...\n";

	std::cin.get();

	return 0;
}