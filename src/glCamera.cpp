#include <glCamera.h>

void GlCamera::processKeyboard(CameraMovement direction, double deltaTime)
{
	float velocity = moveSpeed * (float)deltaTime;

	switch (direction)
	{
	case FORWARD:
		position += front * velocity;
		break;

	case BACKWARD:
		position -= front * velocity;
		break;

	case LEFT:
		position -= right * velocity;
		break;

	case RIGHT:
		position += right * velocity;
		break;

	case UP:
		position += up * velocity;
		break;

	case DOWN:
		position -= up * velocity;
		break;
	}
}

void GlCamera::processMouse(float xOffset, float yOffset)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > constrainEulerAngle)
			pitch = constrainEulerAngle;
		if (pitch < -constrainEulerAngle)
			pitch = -constrainEulerAngle;
	}

	updateCamera();
}

void GlCamera::processMouseScroll(float yOffset)
{
	fieldOfView -= yOffset * scrollSensitivity;

	if (fieldOfView < 1.0f)
		fieldOfView = 1.0f;
	if (fieldOfView > 179.0f)
		fieldOfView = 179.0f;
}

glm::mat4 GlCamera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 GlCamera::getProjectionMatrix() const
{
	if (projectionType == PERSPECTIVE)
	{
		return glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClip, farClip);
	}
	else
	{
		return glm::ortho(-aspectRatio * orthographicSize, aspectRatio * orthographicSize, -orthographicSize, orthographicSize, nearClip, farClip);
	}
}

GlCamera::GlCamera(glm::vec3 position, glm::vec3 up, CameraProjectionType projection, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensitivity(MOUSE_SENSITIVITY), scrollSensitivity(SCROLL_SENSITIVITY), fieldOfView(FOV), orthographicSize(ORTHO_SIZE), 
	constrainPitch(true), constrainEulerAngle(CONSTRAIN_EULER_ANGLE), aspectRatio(1), nearClip(0.1f), farClip(100.0f)
{
	GlCamera::projectionType = projection;

	GlCamera::position = position;
	GlCamera::worldUp = up;

	GlCamera::yaw = yaw;
	GlCamera::pitch = pitch;

	updateCamera();
}

GlCamera::GlCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, CameraProjectionType projection, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), mouseSensitivity(MOUSE_SENSITIVITY), scrollSensitivity(SCROLL_SENSITIVITY), fieldOfView(FOV), orthographicSize(ORTHO_SIZE),
	constrainPitch(true), constrainEulerAngle(CONSTRAIN_EULER_ANGLE), aspectRatio(1), nearClip(0.1f), farClip(100.0f)
{
	GlCamera::projectionType = projection;

	GlCamera::position = glm::vec3(posX, posY, posZ);
	GlCamera::worldUp = glm::vec3(upX, upY, upZ);

	GlCamera::yaw = yaw;
	GlCamera::pitch = pitch;

	updateCamera();
}

void GlCamera::updateCamera()
{
	glm::vec3 frontNew;
	frontNew.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontNew.y = sin(glm::radians(pitch));
	frontNew.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(frontNew);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}