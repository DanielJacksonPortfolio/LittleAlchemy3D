#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

#include "Elements.h"

ElementManager::ElementManager(Font* font) {
	this->font = font;
	this->defaultModel = new Model("undefined");
	this->Load();
}

ElementManager::~ElementManager() {

}

void ElementManager::LoadRecipes() {
	std::ifstream recipeList;
	recipeList.open("res/data/load/recipes.txt");
	if (recipeList.is_open())
	{
		int numResults;
		recipeList >> numResults;
		recipeList >> numRecipes;
		recipes = new Recipe[numRecipes];
		int count = 0;
		for (int i = 0; i < numResults; i++)
		{
			int result, numSubRecipes;
			recipeList >> result;
			recipeList >> numSubRecipes;
			for (int j = 0; j < numSubRecipes; j++)
			{
				int item1, item2;
				recipeList >> item1;
				recipeList >> item2;

				recipes[count].SetItem(item1, 0);
				recipes[count].SetItem(item2, 1);
				recipes[count].SetItem(count, 2);
				recipes[count].SetItem(result, 3);
				count++;
			}
		}
	}
	else
	{
		std::cout << "Error Loading Recipes" << std::endl;
	}
	recipeList.close();
}
void ElementManager::LoadElements()
{
	std::ifstream elementList;
	elementList.open("res/data/load/elements.txt");
	if (elementList.is_open())
	{
		elementList >> numElements;
		elements = new Element[numElements];
		for (int i = 0; i < numElements; i++)
		{
			int buffer, finalItem;
			std::string name;
			elementList >> buffer;
			elementList >> name;
			std::replace(name.begin(), name.end(), '_', ' ');
			elementList >> finalItem;
			elements[i].SetID(i);
			elements[i].SetName(name);
			elements[i].SetFinal(finalItem);
			elements[i].SetModel(new Model(name));
			if (!elements[i].GetModel()->loaded) {
				Rect dest = Rect(0, 0, 100, 100);
				elements[i].text = new Text(name, font, dest, glm::vec3(0.85f, 0.85f, 0.85f), 0.8f, 10.0f);
				elements[i].text->UpdatePositionAndScale(&dest);

			}
			else {
				elementsWithModels.push_back(i);
			}
		}
	}
	else
	{
		std::cout << "Error Loading Elements" << std::endl;
	}
	elementList.close();
}

void ElementManager::Save()
{
	std::ofstream saveFile;
	saveFile.open("res/data/load/save.txt");
	if (saveFile.is_open())
	{
		for (int i = 0; i < numElements; i++)
		{
			saveFile << elementalUnlock[i];
			saveFile << std::endl;
		}

		for (int i = 0; i < numRecipes; i++)
		{
			saveFile << recipeDicovered[i];
			saveFile << std::endl;
		}
	}
	else
	{
		std::cout << "Error Saving" << std::endl;
	}
	saveFile.close();
}

void ElementManager::Load() {
	LoadElements();
	LoadRecipes();
	LoadSaveFile("res/data/load/defaultSave.txt");
}

void ElementManager::LoadSaveFile(std::string filePath, bool metaDataOnly)
{
	std::ifstream saveFile;
	saveFile.open(filePath);

	if (saveFile.is_open())
	{
		std::getline(saveFile, dateSaved);
		saveFile >> numElementsUnlocked;
		saveFile >> numRecipesUnlocked;
		if (!metaDataOnly)
		{
			elementalUnlock = new int[numElements];
			int state = 0;
			for (int i = 0; i < numElements; i++)
			{
				saveFile >> state;
				elementalUnlock[i] = state;
			}
			recipeDicovered = new int[numRecipes];
			for (int i = 0; i < numRecipes; i++)
			{
				saveFile >> state;
				recipeDicovered[i] = state;
			}
		}
	}
	else
	{
		std::cout << "Error Loading" << std::endl;
	}
	saveFile.close();
}


int ElementManager::CombineItems(int itemID1, int itemID2) {
	if (elementalUnlock[itemID1] == 1 && elementalUnlock[itemID2] == 1) {
		int recipeID = GetRecipeID(itemID1, itemID2);
		if (recipeDicovered[recipeID] == 0) {
			recipeDicovered[recipeID] = 1;
			std::cout << elements[recipes[recipeID].GetItem(0)].GetName() << " + " << elements[recipes[recipeID].GetItem(1)].GetName() << " = " << elements[recipes[recipeID].GetItem()].GetName() << std::endl;
			if (elementalUnlock[elements[recipes[recipeID].GetItem()].GetID()] == 0) {
				elementalUnlock[elements[recipes[recipeID].GetItem()].GetID()] = 1;
				return recipes[recipeID].GetItem();
			}
		}
		else if (recipeDicovered[recipeID] == 1) {
			//already been created
			return -1;
		}
		else if (recipeID == -1) {
			//recipe does not exist
			return -2;
		}
	}
	else {
		//not unlocked
		return -3;
	}
}

int ElementManager::GetRecipeID(int item1, int item2) {
	for (int i = 0; i < numRecipes; i++) {
		if (recipes[i].GetItem(0) == item1) {
			if (recipes[i].GetItem(1) == item2) {
				return i;
			}
		}
		else if (recipes[i].GetItem(1) == item1) {
			if (recipes[i].GetItem(0) == item2) {
				return i;
			}
		}
	}
	return -1;
}

int ElementManager::GetIDFromName(std::string name) {
	for (int i = 0; i < numElements; i++) {	
		if (elements[i].GetName() == name) {
			return i;
		}
	}
	return -1;
}