#include "GUISearch.h"

GUISearch::GUISearch(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int maxLength, int width, int height, Widget* searchContainer, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	Rect* dest = new Rect(this->position.x, this->position.y, this->width, this->height);
	this->renderQuad = GUIQuad(dest, colour);
	this->searchContainer = searchContainer;
	this->colour = colour;
	this->colourMode = colourMode;
	this->maxLength = maxLength;
}

GUISearch::~GUISearch()
{
	delete searchContainer;
	delete container;
	delete widgetList;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		delete widgets[i];
	}
	for (size_t i = 0; i < displayWidgets.size(); i++)
	{
		delete displayWidgets[i];
	}
}

void GUISearch::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (x != 0.0f || y != 0.0f)
	{
		storePosition = position;
		position.x += x;
		position.y += y;
	}
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		if (colourMode != TRANSPARENT) {
			renderQuad.Draw(imageShader, x, y);
		}
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->Draw(imageShader, textShader, x, y);
		}
	}
	if (x != 0.0f || y != 0.0f)
	{
		position = storePosition;
	}
}


void GUISearch::HoverControl(bool hovering)
{
	if (hovering && !hovered)
	{
		hovered = true;
		colourMultiplier = 0.75f;
		renderQuad.colour = colour * colourMultiplier;
	}
	else if (!hovering && hovered)
	{
		hovered = false;
		colourMultiplier = 1.0f;
		renderQuad.colour = colour * colourMultiplier;
	}
}

void GUISearch::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	if (x != 0.0f || y != 0.0f)
	{
		storePosition = position;
		position.x += x;
		position.y += y;
	}
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		bool insideThis = false;
		if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
		{
			insideThis = true;
			if (inputManager->leftMouseState == 2) {
				inputManager->leftMouseState = 1;
				if (active)
				{
					HoverControl(false);
					active = false;
				}
				else
				{
					HoverControl(true);
					active = true;
				}
			}
		}
		else
		{
			if (inputManager->leftMouseState == 2) {
				HoverControl(false);
				active = false;
			}
		}
		if (active)
		{
			bool changeMade = false;
			if (inputManager->keyStates["bksp"] == 2) { 
				inputManager->keyStates["bksp"] = 1; 
				if (searchText.length() > 0)
				{
					searchText.pop_back();
					changeMade = true;
				}
			}
			if (inputManager->keyStates["sp"] == 2) { inputManager->keyStates["sp"] = 1; searchText += " "; changeMade = true; }
			if (inputManager->keyStates["a"] == 2) { inputManager->keyStates["a"] = 1; searchText += "A"; changeMade = true; }
			if (inputManager->keyStates["b"] == 2) { inputManager->keyStates["b"] = 1; searchText += "B"; changeMade = true; }
			if (inputManager->keyStates["c"] == 2) { inputManager->keyStates["c"] = 1; searchText += "C"; changeMade = true; }
			if (inputManager->keyStates["d"] == 2) { inputManager->keyStates["d"] = 1; searchText += "D"; changeMade = true; }
			if (inputManager->keyStates["e"] == 2) { inputManager->keyStates["e"] = 1; searchText += "E"; changeMade = true; }
			if (inputManager->keyStates["f"] == 2) { inputManager->keyStates["f"] = 1; searchText += "F"; changeMade = true; }
			if (inputManager->keyStates["g"] == 2) { inputManager->keyStates["g"] = 1; searchText += "G"; changeMade = true; }
			if (inputManager->keyStates["h"] == 2) { inputManager->keyStates["h"] = 1; searchText += "H"; changeMade = true; }
			if (inputManager->keyStates["i"] == 2) { inputManager->keyStates["i"] = 1; searchText += "I"; changeMade = true; }
			if (inputManager->keyStates["j"] == 2) { inputManager->keyStates["j"] = 1; searchText += "J"; changeMade = true; }
			if (inputManager->keyStates["k"] == 2) { inputManager->keyStates["k"] = 1; searchText += "K"; changeMade = true; }
			if (inputManager->keyStates["l"] == 2) { inputManager->keyStates["l"] = 1; searchText += "L"; changeMade = true; }
			if (inputManager->keyStates["m"] == 2) { inputManager->keyStates["m"] = 1; searchText += "M"; changeMade = true; }
			if (inputManager->keyStates["n"] == 2) { inputManager->keyStates["n"] = 1; searchText += "N"; changeMade = true; }
			if (inputManager->keyStates["o"] == 2) { inputManager->keyStates["o"] = 1; searchText += "O"; changeMade = true; }
			if (inputManager->keyStates["p"] == 2) { inputManager->keyStates["p"] = 1; searchText += "P"; changeMade = true; }
			if (inputManager->keyStates["q"] == 2) { inputManager->keyStates["q"] = 1; searchText += "Q"; changeMade = true; }
			if (inputManager->keyStates["r"] == 2) { inputManager->keyStates["r"] = 1; searchText += "R"; changeMade = true; }
			if (inputManager->keyStates["s"] == 2) { inputManager->keyStates["s"] = 1; searchText += "S"; changeMade = true; }
			if (inputManager->keyStates["t"] == 2) { inputManager->keyStates["t"] = 1; searchText += "T"; changeMade = true; }
			if (inputManager->keyStates["u"] == 2) { inputManager->keyStates["u"] = 1; searchText += "U"; changeMade = true; }
			if (inputManager->keyStates["v"] == 2) { inputManager->keyStates["v"] = 1; searchText += "V"; changeMade = true; }
			if (inputManager->keyStates["w"] == 2) { inputManager->keyStates["w"] = 1; searchText += "W"; changeMade = true; }
			if (inputManager->keyStates["x"] == 2) { inputManager->keyStates["x"] = 1; searchText += "X"; changeMade = true; }
			if (inputManager->keyStates["y"] == 2) { inputManager->keyStates["y"] = 1; searchText += "Y"; changeMade = true; }
			if (inputManager->keyStates["z"] == 2) { inputManager->keyStates["z"] = 1; searchText += "Z"; changeMade = true; }
			if (inputManager->keyStates["0"] == 2) { inputManager->keyStates["0"] = 1; searchText += "0"; changeMade = true; }
			if (inputManager->keyStates["1"] == 2) { inputManager->keyStates["1"] = 1; searchText += "1"; changeMade = true; }
			if (inputManager->keyStates["2"] == 2) { inputManager->keyStates["2"] = 1; searchText += "2"; changeMade = true; }
			if (inputManager->keyStates["3"] == 2) { inputManager->keyStates["3"] = 1; searchText += "3"; changeMade = true; }
			if (inputManager->keyStates["4"] == 2) { inputManager->keyStates["4"] = 1; searchText += "4"; changeMade = true; }
			if (inputManager->keyStates["5"] == 2) { inputManager->keyStates["5"] = 1; searchText += "5"; changeMade = true; }
			if (inputManager->keyStates["6"] == 2) { inputManager->keyStates["6"] = 1; searchText += "6"; changeMade = true; }
			if (inputManager->keyStates["7"] == 2) { inputManager->keyStates["7"] = 1; searchText += "7"; changeMade = true; }
			if (inputManager->keyStates["8"] == 2) { inputManager->keyStates["8"] = 1; searchText += "8"; changeMade = true; }
			if (inputManager->keyStates["9"] == 2) { inputManager->keyStates["9"] = 1; searchText += "9"; changeMade = true; }
			if (changeMade)
			{
				for (size_t i = 0; i < widgets.size(); i++)
				{
					std::string type = typeid(*widgets[i]).name();
					type = type.substr(6);
					if (type == "GUIText")
					{
						if (searchText.length() > 0)
						{
							if (searchText.length() > maxLength)
							{
								displayText = searchText.substr(searchText.length() - maxLength);
							}
							else
							{
								displayText = searchText;
							}
						}
						else
						{
							displayText = "Search...";
						}
						widgets[i]->UpdateText(displayText);
					}
				}
				this->searchContainer->SearchWidgets(searchText);
			}
		}
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->Input(insideThis, mousePos, inputManager, x, y);
		}
	}
	if (x != 0.0f || y != 0.0f)
	{
		position = storePosition;
	}
}
