#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glShader.h"
#include "glCamera.h"
#include "glWindow.h"
#include "glModel.h"

#include "atlasGameObj.h"

#include <vector>

class GlRenderer {
public:
    double getDeltaTime();

	void render(std::vector<AtlasGameObj*> gameObjects);
    void addWindow(GlWindow* window, std::vector<AtlasGameObj*> gameObjects);
    void addModel(GlModel* model);

	GlRenderer();
	~GlRenderer();

    GLFWwindow* sharedResources;
    std::vector<GlWindow*> windows;

private:
    void calculateDeltaTime();

    double deltaTime;
    double lastFrame;
    
    glm::mat4 mv;
    glm::mat4 mvp;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    GlShader* lightingShader;
    GlShader* lightCubeShader;

    std::vector<GlModel*> models;
};
