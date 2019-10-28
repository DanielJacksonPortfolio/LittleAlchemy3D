#pragma once

#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

// default camera values
#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_SPEED 7.0f
#define CAM_DEFAULT_SENSITIVITY 0.1f
#define CAM_DEFAULT_ZOOM 70.0f


class Camera
{
public:
	//camera Attributes
	glm::vec3 Position;
	glm::vec3 oldPosition;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//euler angles
	float Yaw, Pitch;
	float oldYaw, oldPitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float previousZoom;

	bool rideCoaster = false;
	unsigned int coasterRow = 6;
	unsigned int coasterSeat = 2;

	glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f), bitangent = glm::vec3(0.0f, 0.0f, 1.0f), normal = glm::vec3(0.0f, 1.0f, 0.0f);

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CAM_DEFAULT_YAW, float pitch = CAM_DEFAULT_PITCH);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessMouseScroll(float yoffset);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();

	void StopRidingCoaster();
};