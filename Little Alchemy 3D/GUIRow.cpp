#include "GUIRow.h"


GUIRow::GUIRow(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode ColourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);

	this->colour = colour;
	this->colourMode = ColourMode;
}

GUIRow::~GUIRow()
{
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

void GUIRow::AutoAdjust()
{
	if (displayWidgets.size() > 0)
	{
		int widgetW = 0;
		for (size_t i = 0; i < displayWidgets.size(); i++)
		{
			widgetW += displayWidgets[i]->GetWidth();
		}
		int spare = this->width - widgetW;
		float gap = spare / (displayWidgets.size() + 1);
		float pos = gap;
		for (size_t i = 0; i < displayWidgets.size(); i++)
		{
			displayWidgets[i]->UpdatePositionData(POSITION_ABSOLUTE, POSITION_PERCENTAGE_CENTRE, pos, 50, true);
			pos += gap;
			pos += displayWidgets[i]->GetWidth();
		}
		totalWidth = pos;
	}
}

void GUIRow::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (x != 0.0f || y != 0.0f)
	{
		storePosition = position;
		position.x += x;
		position.y += y;
	}
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		renderQuad.Draw(imageShader,x,y);
		for (size_t i = 0; i < displayWidgets.size(); i++)
		{
			displayWidgets[i]->Draw(imageShader, textShader, x, y);
		}
	}
	if (x != 0.0f || y != 0.0f)
	{
		position = storePosition;
	}
}

void GUIRow::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	glm::vec2 storePosition = position;
	position.x += x;
	position.y += y;
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		bool insideThis = false;
		if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
		{
			insideThis = true;
		}
		for (size_t i = 0; i < displayWidgets.size(); i++)
		{
			displayWidgets[i]->Input(insideThis, mousePos, inputManager, x, y);
		}
	}
	position = storePosition;
}

void GUIRow::SearchWidgets(std::string searchText)
{
	displayWidgets = {};
	for (size_t i = 0; i < widgets.size(); i++)
	{
		std::string widgetID = widgets[i]->GetID();
		std::transform(widgetID.begin(), widgetID.end(), widgetID.begin(), ::toupper);
		if (widgetID.substr(0, searchText.length()) == searchText || searchText.length() == 0 || searchText == " ")
		{
			displayWidgets.push_back(widgets[i]);
		}
	}
	AutoAdjust();
}