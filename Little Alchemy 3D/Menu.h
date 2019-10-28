#pragma once
#include <vector>
#include <math.h>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "Constants.h"

#include "Page.h"
#include "GUIText.h"
#include "GUIImage.h"
#include "GUIGrid.h"
#include "Button.h"
#include "List.h"
#include "ScrollingList.h"
#include "Widget.h"
#include "Elements.h"
#include "Item.h"

class Menu
{
private:
	std::unordered_map<std::string, std::function<void()>> vFunctionMap = {};
	std::unordered_map<std::string, std::function<void(std::string)>> sFunctionMap = {};

	std::unordered_map<std::string, Widget*> widgetList = {};
	std::unordered_map<std::string, GUITexture*> Textures;
	std::unordered_map<std::string, Font*> Fonts;

	GLFWwindow* window;
	ElementManager* elementManager;
	ItemManager* itemManager;

	std::string setSaveID = "1";
	std::string dateSaved = "n/a";

public:
	bool showMenu = true, showSettings = false, showEGrid = false, showLoadMenu = false, showPauseMenu = false, wasPaused = false;

	Menu(GLFWwindow* window, ElementManager* elementManager, ItemManager* itemManager);
	~Menu();
	bool Input(glm::vec2 mousePos, InputManager* inputManager);
	void Draw(Shader* imageShader, Shader* textShader);
	bool LoadContent();
	bool LoadFonts();
	bool LoadTextures();
	void Add(std::string containerID, std::string id, WidgetType widgetType, std::vector<std::pair<std::string, std::string>> valuePairs);
	void Null();
	void Null(std::string string);
	void Quit();
	void Settings();
	void PlayGame();
	void LoadMenu();
	void Resume();
	void MainMenu();
	void UpdateElementList();
	void SpawnElement(std::string ID);

	void SetSave(std::string saveID);
	void LoadSave();
};