#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glModel.h"

class AtlasGameObj {
public:
	void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }
	void setPosition(glm::vec3 position) { AtlasGameObj::position = position; }

	void setScale(float x, float y, float z) { scale = glm::vec3(x, y, z); }
	void setScale(glm::vec3 scale) { AtlasGameObj::scale = scale; }

	void setRotation(float x, float y, float z) { rotation = glm::vec3(x, y, z); }
	void setRotation(glm::vec3 rotation) { AtlasGameObj::rotation = rotation; }

	AtlasGameObj(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f), GlModel* model = nullptr)
		: position(position), scale(scale), rotation(rotation), model(model) {}
	~AtlasGameObj() {};

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	GlModel* model;

private:

};