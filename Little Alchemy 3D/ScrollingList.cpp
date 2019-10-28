#include "ScrollingList.h"

ScrollingList::ScrollingList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container,ScrollMode scrollMode, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	Rect* dest = new Rect(this->position.x, this->position.y, this->width, this->height);
	this->renderQuad = GUIQuad(dest, colour);
	this->colour = colour;
	this->colourMode = colourMode;
	this->scrollMode = scrollMode;
}


ScrollingList::~ScrollingList()
{
	delete container;
	delete widgetList;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		delete widgets[i];
	}
	for (size_t i = 0; i < lists.size(); i++)
	{
		delete lists[i];
	}
}

void ScrollingList::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		if (colourMode != TRANSPARENT) {
			renderQuad.Draw(imageShader, x, y);
		}
		//SDL_RenderSetClipRect(renderer, &rect);

		switch (scrollMode)
		{
		case LIST:
			for (size_t i = 0; i < widgets.size(); i++)
			{
				if (i == showList)
					widgets[i]->Draw(imageShader, textShader, x, y);
			}
			break;
		case SMOOTH:
			for (size_t i = 0; i < displayWidgetAmount; i++)
			{
				displayWidgets[i]->Draw(imageShader, textShader, 0, -viewY);
			}
			break;
		}
		//SDL_RenderSetClipRect(renderer, NULL);
	}
}


void ScrollingList::AutoAdjust()
{
	bool complete = false;
	displayWidgetAmount = displayWidgets.size();
	switch (scrollMode)
	{
	case LIST:
		while (!complete && widgets.size() > 0)
		{
			int widgetH = 0;
			for (size_t i = 0; i < widgets.size(); i++)
			{
				widgetH += widgets[i]->GetHeight();
			}
			float gap = widgetH / (widgets.size() * 2);

			float pageHeight = gap;
			int setCap = 0;
			for (size_t i = 0; i < widgets.size(); i++)
			{
				pageHeight += widgets[i]->GetHeight();
				pageHeight += gap;
				if (pageHeight > this->height)
				{
					setCap = i--;
					break;
				}
				else if (i == widgets.size() - 1)
				{
					setCap = i;
					complete = true;
				}
			}

			lists.push_back(new List(this->widgetList, this->ID + " - List " + std::to_string(++pageCount), this, 100, 100, DIMENSION_PERCENTAGE, DIMENSION_PERCENTAGE, this->alignment, glm::vec2(0.0f,0.0f), POSITION_ABSOLUTE, POSITION_ABSOLUTE, this->colour, this->colourMode));
			for (size_t i = 0; i <= setCap; i++)
			{
				(*lists[pageCount - 1]->GetWidgets()).push_back(widgets[i]);
			}
			for (size_t i = 0; i <= setCap; i++)
			{
				std::string widgetID = widgets[0]->GetID();
				(*widgetList).erase(widgetID);
				this->widgets.erase(widgets.begin());
			}
		}
		for (size_t i = 0; i < lists.size(); i++)
		{
			lists[i]->AutoAdjust();
		}
		for (size_t i = 0; i < lists.size(); i++)
		{
			widgets.push_back(lists[i]);
		}
		break;
	case SMOOTH:
		int widgetH = 0;
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgetH += widgets[i]->GetHeight();
		}
		float gap = widgetH / (widgets.size() * 2.0f);
		float pos = gap;
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->UpdatePositionData(POSITION_PERCENTAGE_CENTRE, POSITION_ABSOLUTE, 50, pos, true);
			pos += gap;
			pos += widgets[i]->GetHeight();
		}
		totalHeight = pos;
		break;
	}
}

void ScrollingList::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		bool insideThis = false;
		if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
		{
			insideThis = true;

			if (inputManager->scrollOffset < 0)
			{
				switch (scrollMode)
				{
				case LIST:
					if (showList != 0)
					{
						showList--;
					}
					break;
				case SMOOTH:
					if (viewY < totalHeight - this->height)
					{
						viewY -= inputManager->scrollOffset * 100.0f;
						inputManager->scrollOffset = 0.0f;
					}
					break;
				}
			}
			else if (inputManager->scrollOffset > 0) {
				switch (scrollMode)
				{
				case LIST:
					if (showList != lists.size() - 1)
					{
						showList++;
					}
					break;
				case SMOOTH:
					if (viewY > 0.0f)
					{
						viewY -= inputManager->scrollOffset * 100.0f;
						inputManager->scrollOffset = 0.0f;
					}
					break;
				}
			}
		}
		if (viewY <= 0.0f)
		{
			viewY = 0.0f;
		}
		if (viewY > totalHeight - this->height && totalHeight > this->height)
		{
			viewY = totalHeight - this->height;
		}
		switch (scrollMode)
		{
		case LIST:
			for (size_t i = 0; i < widgets.size(); i++)
			{
				if (i == showList)
					widgets[i]->Input(insideThis, mousePos, inputManager);
			}
		case SMOOTH:
			for (size_t i = 0; i < displayWidgetAmount; i++)
			{
				displayWidgets[i]->Input(insideThis, mousePos, inputManager, 0, -viewY);
			}
			break;
		}
	}
}

void ScrollingList::SearchWidgets(std::string searchText)
{
	displayWidgets = {};
	for (size_t i = 0; i < widgets.size(); i++)
	{
		std::string type = typeid(*widgets[i]).name();
		type = type.substr(6);
		std::string widgetID = widgets[i]->GetID();
		std::transform(widgetID.begin(), widgetID.end(), widgetID.begin(), ::toupper);
		if ((widgetID.substr(0, searchText.length()) == searchText || searchText.length() == 0 || type == "GUISearch" || searchText == " ") && type != "GUIRow")
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
	}
	AutoAdjust();
}