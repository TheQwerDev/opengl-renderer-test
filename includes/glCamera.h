#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum CameraProjectionType {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float MOUSE_SENSITIVITY = 0.1f;
const float SCROLL_SENSITIVITY = 2.0f;
const float FOV = 45.0f;
const float ORTHO_SIZE = 3.0f;
const float CONSTRAIN_EULER_ANGLE = 89.0f;

class GlCamera {
public:
	void processKeyboard(CameraMovement direction, double deltaTime);
	void processMouse(float xOffset, float yOffset);
	void processMouseScroll(float yOffset);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	GlCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), CameraProjectionType projection = PERSPECTIVE, float yaw = YAW, float pitch = PITCH);
	GlCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, CameraProjectionType projection = PERSPECTIVE, float yaw = YAW, float pitch = PITCH);

	CameraProjectionType projectionType;
	float aspectRatio;
	float nearClip;
	float farClip;

	float moveSpeed;
	float mouseSensitivity;
	float scrollSensitivity;

	float fieldOfView;
	float orthographicSize;

	float yaw;
	float pitch;

	bool constrainPitch;
	float constrainEulerAngle;

	glm::vec3 position;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 right;

private:
	void updateCamera();
};