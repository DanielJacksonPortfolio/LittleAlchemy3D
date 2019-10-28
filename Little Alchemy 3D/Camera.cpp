#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(CAM_DEFAULT_SPEED), MouseSensitivity(CAM_DEFAULT_SENSITIVITY), Zoom(CAM_DEFAULT_ZOOM), previousZoom(CAM_DEFAULT_ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(CAM_DEFAULT_SPEED), MouseSensitivity(CAM_DEFAULT_SENSITIVITY), Zoom(CAM_DEFAULT_ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);//+ viewOffset
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset * (Zoom / 90.0f);
	Pitch += yoffset * (Zoom / 90.0f);

	//constrain pitch
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	//update axis
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
	/*glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);*/

	Front = bitangent * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)) + tangent * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)) + normal * sin(glm::radians(Pitch));
	Right = glm::normalize(glm::cross(Front, WorldUp)); 
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::StopRidingCoaster() {
	rideCoaster = false;
	tangent = glm::vec3(1.0f, 0.0f, 0.0f);
	bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Position = oldPosition;
	Yaw = oldYaw;
	Pitch = oldPitch;
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
	if (Zoom >= 1.0f && Zoom <= 110.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 110.0f)
		Zoom = 110.0f;
}