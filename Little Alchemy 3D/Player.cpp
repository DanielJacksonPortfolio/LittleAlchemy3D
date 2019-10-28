#include "Player.h"

Player::Player(Camera* camera)
{
	this->camera = camera;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}


Player::~Player()
{
}

void Player::Update(float deltaTime) {
	velocity.y -= 5.0f * deltaTime;
	position += velocity * deltaTime;
	if (position.y < headHeight) {
		position.y = headHeight;
		velocity.y = 0.0f;
	}
	CheckCollisions();
	velocity.x *= 1.0f - 10.0f * deltaTime;
	velocity.z *= 1.0f - 10.0f * deltaTime;
	velocity.y *= 1.0f - 0.5f * deltaTime;
	camera->Position = position;
}

void Player::CheckCollisions() {
	float distance = ExtraMaths::Dist(position, glm::vec3(0.0f, 0.0f, 0.0f));
	//if (distance > 13.0f) {
	//	float angle = atan2(position.z, position.x);
	//	//position.x = cos(angle) * 15.0f;
	//	//position.z = sin(angle) * 15.0f;

	//	velocity.x -= cos(angle) * 0.3f * (distance - 13.0f);
	//	velocity.z -= sin(angle) * 0.3f * (distance - 13.0f);
	//}
}

void Player::ProcessKeyboard(PlayerMovement action, float deltaTime, float speedMod) {
	float scalarVelocity = 60.0f * deltaTime * speedMod;
	if (action == PM_FORWARD)
		velocity += glm::vec3(cos(glm::radians(camera->Yaw)), 0.0f, sin(glm::radians(camera->Yaw))) * scalarVelocity;
	if (action == PM_BACKWARD)
		velocity -= glm::vec3(cos(glm::radians(camera->Yaw)), 0.0f, sin(glm::radians(camera->Yaw))) * scalarVelocity;
	if (action == PM_LEFT)
		velocity -= camera->Right * scalarVelocity;
	if (action == PM_RIGHT)
		velocity += camera->Right * scalarVelocity;
	if (action == PM_JUMP && position.y <= headHeight)
		velocity.y += scalarVelocity * 20.0f;
}