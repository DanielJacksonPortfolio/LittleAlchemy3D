#pragma once

#include "Constants.h"
#include "Camera.h"


#include <glm.hpp>

enum PlayerMovement {
	PM_FORWARD, PM_BACKWARD, PM_LEFT, PM_RIGHT, PM_JUMP
};

class Player
{
private:
	Camera* camera;
	float headHeight = 2.5f;
public:
	glm::vec3 position;
	glm::vec3 velocity;

	Player(Camera* camera);
	~Player();
	void Update(float deltaTime);
	void ProcessKeyboard(PlayerMovement action, float deltaTime, float speedMod);
	void CheckCollisions();
};

