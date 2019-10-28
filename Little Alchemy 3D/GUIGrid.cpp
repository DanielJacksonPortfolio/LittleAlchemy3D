#include "GUIGrid.h"
#include <algorithm>
#include <string>

GUIGrid::GUIGrid(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, ScrollMode scrollMode, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode, int rowCount, RowModes rowMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	this->colour = colour;
	this->colourMode = colourMode;
	this->rowMode = rowMode;
	switch (this->rowMode)
	{
	case WIDGET:
	case PIXEL:
		this->rowCount = rowCount;
		break;
	case PERCENTAGE:
		this->rowCount = StaticMenuFuncs::roundUp((this->container->GetWidth() * (rowCount * 0.01f)), 1);
		break;
	}
	this->list = new ScrollingList(this->widgetList, this->ID + " - List ", this, SMOOTH, 100, 100, DIMENSION_PERCENTAGE, DIMENSION_PERCENTAGE, this->alignment, { 0, 0 }, POSITION_ABSOLUTE, POSITION_ABSOLUTE, this->colour, this->colourMode);

}


GUIGrid::~GUIGrid()
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
	for (size_t i = 0; i < rows.size(); i++)
	{
		delete rows[i];
	}
	delete search;
	delete list;
}

void GUIGrid::AutoAdjust()
{
	rows = {};
	if (displayWidgets.size() > 0)
	{
		switch (rowMode)
		{
		case WIDGET:
			int widgetCount = 0;
			int numberOfRows = ceil(static_cast<float>(displayWidgets.size()) / rowCount);
			for (int j = 0; j < numberOfRows; j++)
			{
				rows.push_back(new GUIRow(widgetList, ID + "ROW" + std::to_string(j), this, 100, 100, DIMENSION_PERCENTAGE, DIMENSION_ABSOLUTE, ALIGNMENT_ABSOLUTE, { 50, 0 }, POSITION_PERCENTAGE_CENTRE, POSITION_ABSOLUTE, colour, colourMode));
				for (int i = 0; i < rowCount; i++)
				{
					if (widgetCount < displayWidgets.size())
					{
						(*rows[j]->GetWidgets()).push_back(displayWidgets[widgetCount++]);
						(*rows[j]->GetWidgets())[i]->SetContainer(rows[j]);
					}
					else
					{
						break;
					}
				}
			}
			break;
			//case PIXEL:
			//	break;
			//case PERCENTAGE:
			//	break;
			//default:
			//	break;
		}
	}
	(*list->GetWidgets()) = {};
	if (search != nullptr)
	{
		(*list->GetWidgets()).push_back(search);
		(*list->GetWidgets()).back()->SetContainer(list);
	}
	if (rows.size() > 0)
	{
		for (int i = 0; i < rows.size(); i++)
		{
			rows[i]->SetAllWidgets();
			(*list->GetWidgets()).push_back(rows[i]);
		}
	}
	list->SetAllWidgets();
}

void GUIGrid::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		if (colourMode != TRANSPARENT) {
			renderQuad.Draw(imageShader, x, y);
		}
		list->Draw(imageShader, textShader, x, y);
	}
}

void GUIGrid::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		bool insideThis = false;
		if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
		{
			insideThis = true;
		}
		list->Input(insideThis, mousePos, inputManager, x, y);
	}

}

void GUIGrid::SearchWidgets(std::string searchText)
{
	displayWidgets = {};
	for (size_t i = 0; i < widgets.size(); i++)
	{
		std::string type = typeid(*widgets[i]).name();
		type = type.substr(6);
		std::string widgetID = widgets[i]->GetID();
		std::transform(widgetID.begin(), widgetID.end(), widgetID.begin(), ::toupper);
		if ((widgetID.substr(0, searchText.length()) == searchText || searchText.length() == 0 || searchText == " ") && type != "GUISearch" && type != "GUIRow")
		{
			displayWidgets.push_back(widgets[i]);
		}
		else if (type == "GUIRow")
		{
			widgets[i]->SearchWidgets(searchText);
			if (widgets[i]->GetDisplayWidgets()->size() > 0)
			{
				displayWidgets.push_back(widgets[i]);
			}
		}
		else if (type == "GUISearch")
		{
			this->search = widgets[i];
		}
	}
	AutoAdjust();
}