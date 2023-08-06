#pragma once

#include "glRenderer.h"
#include "atlasGameObj.h"

#include <vector>

class AtlasInstance {
public:
	void update() { renderer->render(gameObjects); }

	AtlasGameObj* newGameObj(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f), GlModel* model = nullptr) 
	{
		AtlasGameObj* newGameObject = new AtlasGameObj(position, scale, rotation, model); 
		gameObjects.push_back(newGameObject);

		return newGameObject;
	}
	void addGameObj(AtlasGameObj* gameObject) { gameObjects.push_back(gameObject); }

	GlWindow* newWindow(const uint32_t& width, const uint32_t& height, const char* name, GlCamera* camera, bool wireframeMode = false)
	{
		GlWindow* newWindow = new GlWindow(width, height, name, camera, renderer->sharedResources, wireframeMode);
		renderer->addWindow(newWindow, gameObjects);

		return newWindow;
	}
	void addWindow(GlWindow* window) { renderer->addWindow(window, gameObjects); }

	GlModel* newModel(const std::string& path) 
	{
		GlModel* newModel = new GlModel(path, renderer->sharedResources);
		renderer->addModel(newModel); 

		return newModel;
	}
	void addModel(GlModel* model) { renderer->addModel(model); }

	double getDeltaTime() { return renderer->getDeltaTime(); }
	GLFWwindow* getRendererResourcesPtr() { return renderer->sharedResources; }

	AtlasInstance() { glfwInit(); renderer = new GlRenderer(); };
	~AtlasInstance() { glfwTerminate(); };

private:
	GlRenderer* renderer = nullptr;

	std::vector<AtlasGameObj*> gameObjects;
};