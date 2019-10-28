#include "Item.h"



Item::Item(ElementManager* elementManager, ItemManager* itemManager, Camera* camera, glm::vec3 position, int type) {
	this->elementManager = elementManager;
	this->itemManager = itemManager;
	this->camera = camera;
	this->position = position;
	this->type = type;
	
	model = elementManager->GetElements()[type].GetModel();
	if (!model->loaded) {
		model = elementManager->GetDefaultModel();
		modelExists = false;
	}
}

Item::~Item()
{
}

void Item::Update(float deltaTime, int itemVectorIndex, bool hovering, float draggingDist) {
	selected = hovering;
	bool applyVelocity = true;
	if (!deleting && draggingDist > 0.0f) {
		glm::vec3 target = camera->Position + camera->Front * draggingDist;
		position += deltaTime * 10.0f * (target - position);
		velocity =  15.0f * (target - position);
	}
	else if (!deleting) {
		float distToCombine1 = ExtraMaths::Dist(position, combinePos1);
		if ((itemManager->combineItem1 == itemVectorIndex || itemManager->combineItem1 == -1) && distToCombine1 < 2.0f) {
			glm::vec3 diff = position - combinePos1;
			position -= deltaTime * 7.5f * diff;
			applyVelocity = false;
			if (distToCombine1 < 0.5f) {
				itemManager->combineItem1 = itemVectorIndex;
			}
		}
		float distToCombine2 = ExtraMaths::Dist(position, combinePos2);
		if ((itemManager->combineItem2 == itemVectorIndex || itemManager->combineItem2 == -1) && distToCombine2 < 2.0f) {
			glm::vec3 diff = position - combinePos2;
			position -= deltaTime * 7.5f * diff;
			applyVelocity = false;
			if (distToCombine2 < 0.5f) {
				itemManager->combineItem2 = itemVectorIndex;
			}
		}
		float distToDelete = ExtraMaths::Dist(position, deletePos);
		if (distToDelete < 2.0f) {
			glm::vec3 diff = position - deletePos;
			position -= deltaTime * 7.5f * diff;
			applyVelocity = false;
			collideable = false;
			velocity = glm::vec3(0.0f);
			if (distToDelete < 0.5f) {
				Delete(3.0f);
				falling = true;
			}
		}
	}
	if (deleting) {
		deleteTick += deltaTime;
		if (deleteTick > maxDeleteDuration) {
			deleted = true;
		}
	}
	//gravity
	if (applyVelocity) {
		velocity.y -= 15.0f * deltaTime;
	
		if (!falling && position.y < floatHeight) {
			velocity.y += (floatHeight - position.y) * deltaTime * 13.0f;
		}
		else {
			glm::vec2 diff = glm::vec2(position.x, position.z) - glm::vec2(deletePos.x, deletePos.z);
			diff = glm::normalize(diff);
			if (diff.x == diff.x) {
				velocity.x -= diff.x * 10.0f * deltaTime;
				velocity.z -= diff.y * 10.0f * deltaTime;
			}
		}

		velocity.x *= 1.0f - 3.25f * deltaTime;
		velocity.z *= 1.0f - 3.25f * deltaTime;
		velocity.y *= 1.0f - 1.25f * deltaTime;

		position += velocity * deltaTime;
	}
}

void Item::Draw(Shader* materialShader, bool setMaterialData) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position);
	if (deleting) {
		float scale = 1.0f - (deleteTick / maxDeleteDuration);
		if (scale < 0.0f) scale = 0.0f;
		else if (scale > 1.0f) scale = 1.0f;
		modelMat = glm::scale(modelMat, glm::vec3(scale));
	}
	materialShader->use();
	materialShader->setMat4("model", modelMat);
	model->Draw(materialShader, setMaterialData);
	
}
void Item::DrawItemText(Shader* textShader) {
	if (!modelExists) {
		float yaw;
		glm::vec3 diff = position - camera->Position;
		yaw = atan2(diff.x, diff.z);

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position + glm::vec3(0.0f, 1.6f, 0.0f));
		modelMat = glm::rotate(modelMat, yaw + (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::scale(modelMat, glm::vec3(elementManager->GetElements()[type].text->scale.x, elementManager->GetElements()[type].text->scale.y, 1.0f));
		modelMat = glm::scale(modelMat, glm::vec3(20.0f));
		//modelMat = glm::translate(modelMat, glm::vec3(-elementManager->GetElements()[type].text->width * 0.5f, 0.0f, 0.0f));

		textShader->use();
		textShader->setMat4("model", modelMat);
		//std::cout << elementManager->GetElements()[type].text->verts.at(0).position.x << std::endl;
		elementManager->GetElements()[type].text->Draw(textShader);
	}
}

void Item::Delete(float deleteDuration) {
	this->deleting = true;
	this->deleteTick = 0.0f;
	this->maxDeleteDuration = deleteDuration;
}

ItemManager::ItemManager(ElementManager* elementManager, Camera* camera, InputManager* inputManager) {
	this->elementManager = elementManager;
	this->camera = camera;
	this->inputManager = inputManager;
}

ItemManager::~ItemManager() {

}

void ItemManager::DrawItems(Shader* materialShader, bool setMaterialData) {
	for (int i = 0; i < items.size(); i++) {
		items.at(i)->Draw(materialShader, setMaterialData);
	}
}

void ItemManager::DrawItemTexts(Shader* textShader) {
	for (int i = 0; i < items.size(); i++) {
		items.at(i)->DrawItemText(textShader);
	}
}

void ItemManager::UpdateItems(float deltaTime, bool takeInput) {
	itemLookingAtIndex = GetItemLookingAt();
	combineItem1 = -1;
	combineItem2 = -1;

	//check if dragging needs to begin
	if (takeInput && !draggingItem && inputManager->leftMouseState > 0 && itemLookingAtIndex != -1) {
		draggingItem = true;
		draggingItemIndex = itemLookingAtIndex;
		draggingDist = ExtraMaths::Dist(camera->Position,items.at(itemLookingAtIndex)->position);
		if (draggingDist > DRAGGING_DISTANCE) draggingDist = DRAGGING_DISTANCE;
	}

	//main update items
	std::vector<int> itemsToRemove;
	for (int i = 0; i < items.size(); i++) {
		bool select = false;
		if (i == itemLookingAtIndex) select = true;
		if (draggingItem && i == draggingItemIndex) {
			items.at(i)->Update(deltaTime,i, true, draggingDist);
		}
		else {
			items.at(i)->Update(deltaTime,i, select);
		}
		if (items.at(i)->deleted) {
			itemsToRemove.push_back(i);
		}
	}
	//delete if necessary
	for (int i = itemsToRemove.size() - 1; i >= 0; i--) {
		items.erase(items.begin() + itemsToRemove.at(i));
	}

	//do collisions
	CollideItems(deltaTime);

	//check if dragging has stopped
	if (inputManager->leftMouseState == 0) {
		draggingItem = false;
		draggingItemIndex = -1;
		draggingDist = -1.0f;
	}
	//std::cout << combineItem1 << " " << combineItem2 << std::endl;
	if (combineItem1 != -1 && combineItem2 != -1) {
		if (combineItem1 < items.size() && combineItem2 < items.size()) {
			int result = elementManager->CombineItems(items.at(combineItem1)->type, items.at(combineItem2)->type);
			//std::cout << elementManager->GetElements()[items.at(combineItem1)->type].GetName() << " " << elementManager->GetElements()[items.at(combineItem2)->type].GetName() << " " << result << std::endl;
			if (result >= 0) {
				items.at(combineItem1)->Delete(0.3f);
				items.at(combineItem2)->Delete(0.3f);
				AddItem(glm::vec3(0.0f, 3.0f, -2.5f), result);
				combineItem1 = -1;
				combineItem2 = -1;
			}
		}
	}
}

bool ItemManager::AddItem(glm::vec3 position, int type) {
	if (items.size() < MAX_ITEM_COUNT) {
		items.push_back(new Item(elementManager, this, camera, position, type));
		return true;
	}
	return false;
}

int ItemManager::GetItemLookingAt() {
	int hitIndex = -1;
	float shortestHitDist = FLT_MAX;
	for (int i = 0; i < items.size(); i++) {
		if (items.at(i)->collideable) {
			if (ExtraMaths::shortestDistSegment(camera->Position, camera->Position + camera->Front * 10.0f, items.at(i)->position) < 1.0f) {
				float dist = ExtraMaths::Dist(camera->Position, items.at(i)->position);
				if (dist < shortestHitDist) {
					shortestHitDist = dist;
					hitIndex = i;
				}
			}
		}
	}
	return hitIndex;
}

void ItemManager::CollideItems(float deltaTime) {
	for (int i = 0; i < items.size(); i++) {
		if (items.at(i)->collideable) {
			for (int j = i; j < items.size(); j++) {
				if (items.at(j)->collideable) {
					float dist = ExtraMaths::Dist(items.at(i)->position, items.at(j)->position);
					if (dist < 2.0f) {
						glm::vec3 diff = items.at(i)->position - items.at(j)->position;
						diff = glm::normalize(diff);
						if (diff.x == diff.x) {
							items.at(i)->velocity += diff * 13.0f * deltaTime * (2.0f - dist);
							items.at(j)->velocity -= diff * 13.0f * deltaTime * (2.0f - dist);
						}
					}
				}
			}
		}
	}
}