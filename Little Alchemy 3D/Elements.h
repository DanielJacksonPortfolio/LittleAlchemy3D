#pragma once

#include "Model.h"
#include "Text.h"

#include <string>

class Recipe
{
private:
	int ID, item1, item2, result;
	bool discovered = false;
public:
	Recipe() {}
	Recipe(int item1, int item2, int result, int ID) 
	{
		this->item1 = item1;
		this->item2 = item2;
		this->result = result;
		this->ID = ID;
	}
	int GetItem(int itemNum = 3)
	{
		switch (itemNum)
		{
		case 0:
			return item1;
			break;
		case 1:
			return item2;
			break;
		case 2:
			return ID;
			break;
		default:
			return result;
			break;
		}
	}
	void SetItem(int val, int itemNum = 3)
	{
		switch (itemNum)
		{
		case 0:
			item1 = val;
			break;
		case 1:
			item2 = val;
			break;
		case 2:
			ID = val;
			break;
		default:
			result = val;
			break;
		}
	}
};

class Element
{
private:
	bool finalItem = false;
	std::string name = "na";
	Model* model;
	int ID = -1;
public:
	Text* text;

	Element() {}
	Element(int ID, std::string name, bool finalItem)
	{
		this->ID = ID;
		this->name = name;
		this->finalItem = finalItem;
	}
	int GetID()
	{
		return ID;
	}
	std::string GetName()
	{
		return name;
	}
	std::string GetFinal()
	{
		if (finalItem)
			return "Final Item";
		else
			return "Not Final Item";
	}
	void SetID(int ID)
	{
		this->ID = ID;
	}
	void SetName(std::string name)
	{
		this->name = name;
	}
	void SetFinal(bool finalItem = false)
	{
		this->finalItem = finalItem;
	}
	void SetModel(Model* model) {
		this->model = model;
	}
	Model* GetModel() {
		return this->model;
	}
};

class ElementManager {
private:
	Recipe* recipes = nullptr;
	Element* elements = nullptr;
	Model* defaultModel = nullptr;
	Font* font = nullptr;

	int* elementalUnlock = nullptr;
	int* recipeDicovered = nullptr;
	int numElements = -1;
	int numRecipes = -1;

	std::string dateSaved = "n/a";
	int numElementsUnlocked = 5;
	int numRecipesUnlocked = 0;
	
public:
	std::vector<unsigned int> elementsWithModels;

	ElementManager(Font* font);
	~ElementManager();
	void Load();
	void Save();
	void LoadRecipes();
	void LoadElements();
	void LoadSaveFile(std::string filePath, bool metaDataOnly = false);
	int CombineItems(int itemID1, int itemID2);
	int GetRecipeID(int item1, int item2);
	int GetIDFromName(std::string name);
	int GetNumElements() { return numElements; }
	int GetNumRecipes() { return numRecipes; }
	int GetNumElementsUnlocked() { return numElementsUnlocked; }
	int GetNumRecipesUnlocked() { return numRecipesUnlocked; }
	Element* GetElements() { return elements; }
	int* GetElementUnlocks() { return elementalUnlock; }
	Model* GetDefaultModel() { return this->defaultModel; }
};
