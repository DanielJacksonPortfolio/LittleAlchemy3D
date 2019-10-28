#pragma once

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "Elements.h"
#include "Camera.h"
#include "Constants.h"

struct ItemManager;
class Item
{
private:
	ElementManager* elementManager;
	ItemManager* itemManager;
	Camera* camera;
	Model* model;
	Text* text;
	bool modelExists = true;
	float floatHeight = 2.5f;
	bool selected = false;
	bool deleting = false;
	bool falling = false;
	float deleteTick;
	float maxDeleteDuration;

	glm::vec3 combinePos1 = glm::vec3(-2.4f, 1.5f, -3.1f);
	glm::vec3 combinePos2 = glm::vec3(2.4f, 1.5f, -3.1f);
	glm::vec3 deletePos = glm::vec3(0.0f, 1.5f, 6.0f);
public:
	int type;
	glm::vec3 position;
	glm::vec3 velocity;
	bool deleted = false;
	bool collideable = true;

	Item(ElementManager* elementManager, ItemManager* itemManager, Camera* camera, glm::vec3 position, int type);
	~Item();
	void Update(float deltaTime, int itemVectorIndex, bool hovering = false, float draggingDist = -1.0f);
	void Draw(Shader* materialShader, bool setMaterialData = true);
	void DrawItemText(Shader* textShader);
	void Delete(float deleteDuration);
};

struct ItemManager {
private:
	std::vector<Item*> items;
	ElementManager* elementManager;
	Camera* camera;
	InputManager* inputManager;
	int itemLookingAtIndex;
	bool draggingItem = false;
	int draggingItemIndex = -1;
	float draggingDist = -1.0f;
public:
	int combineItem1, combineItem2;

	ItemManager(ElementManager* elementManager, Camera* camera, InputManager * inputManager);
	~ItemManager();
	void DrawItems(Shader* materialShader, bool setMaterialData = true);
	void DrawItemTexts(Shader* textShader);
	void UpdateItems(float deltaTime, bool takeInput);
	bool AddItem(glm::vec3 position, int type);
	int GetItemLookingAt();
	void CollideItems(float deltaTime);
};

