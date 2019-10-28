#include "Widget.h"



Widget::Widget()
{
}

void Widget::LoadDefaultData(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY)
{
	LoadInformation(widgetList,ID,container);
	LoadDimensionData(width, height, widthMode, heightMode);
	LoadPositionData(alignment, position, positionModeX, positionModeY);
}

void Widget::LoadDimensionData(int width, int height, DimensionType widthMode, DimensionType heightMode)
{
	this->widthMode = widthMode;
	this->heightMode = heightMode;

	switch (this->widthMode)
	{
	case DIMENSION_ABSOLUTE:
		this->width = width; break;
	case DIMENSION_PERCENTAGE:
		this->width = StaticMenuFuncs::roundUp((this->container->GetWidth() * (width * 0.01f)), 1); break;
	}

	switch (this->heightMode)
	{
	case DIMENSION_ABSOLUTE:
		this->height = height; break;
	case DIMENSION_PERCENTAGE:
		this->height = StaticMenuFuncs::roundUp((this->container->GetHeight() * (height * 0.01f)), 1); break;
	}

}

void Widget::LoadInformation(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container)
{
	this->widgetList = widgetList;
	this->ID = ID;
	(*widgetList)[ID] = this;

	this->container = container;
}

void Widget::LoadPositionData(AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY)
{
	this->alignment = alignment;
	this->positionModeX = positionModeX;
	this->positionModeY = positionModeY;
	this->originalPosition = position;

	switch (this->alignment)
	{
	case TOP_LEFT:
		this->position = glm::vec2 (0.0f,0.0f); break;
	case TOP:
		this->position = glm::vec2(this->container->GetWidth() * 0.5f - this->width * 0.5f,0); break;
	case TOP_RIGHT:
		this->position = glm::vec2(this->container->GetWidth() - this->width,0); break;
	case LEFT:
		this->position = glm::vec2(0,this->container->GetHeight() * 0.5f - this->height * 0.5f); break;
	case CENTRE:
		this->position = glm::vec2(this->container->GetWidth() * 0.5f - this->width * 0.5f,this->container->GetHeight() * 0.5f - this->height * 0.5f);		break;
	case RIGHT:
		this->position = glm::vec2(this->container->GetWidth() - this->width, this->container->GetHeight() * 0.5f - this->height * 0.5f); break;
	case BOTTOM_LEFT:
		this->position = glm::vec2(0, this->container->GetHeight() - this->height); break;
	case BOTTOM:
		this->position = glm::vec2(this->container->GetWidth() * 0.5f - this->width * 0.5f, this->container->GetHeight() - this->height); break;
	case BOTTOM_RIGHT:
		this->position = glm::vec2(this->container->GetWidth() - this->width, this->container->GetHeight() - this->height); break;
	case ALIGNMENT_ABSOLUTE:
		switch (this->positionModeX)
		{
		case POSITION_ABSOLUTE:
			this->position.x = originalPosition.x; break;
		case POSITION_PERCENTAGE_LEFT:
			this->position.x = StaticMenuFuncs::roundUp((this->container->GetWidth() * (originalPosition.x * 0.01f)), 1); break;
		case POSITION_PERCENTAGE_CENTRE:
			this->position.x = StaticMenuFuncs::roundUp((this->container->GetWidth() * (originalPosition.x * 0.01f)), 1) - this->width * 0.5f; break;
		case POSITION_PERCENTAGE_RIGHT:
			this->position.x = StaticMenuFuncs::roundUp((this->container->GetWidth() * (originalPosition.x * 0.01f)), 1) - this->width; break;
		}
		switch (this->positionModeY)
		{
		case POSITION_ABSOLUTE:
			this->position.y = originalPosition.y; break;
		case POSITION_PERCENTAGE_LEFT:
			this->position.y = StaticMenuFuncs::roundUp((this->container->GetHeight() * (originalPosition.y * 0.01f)), 1); break;
		case POSITION_PERCENTAGE_CENTRE:
			this->position.y = StaticMenuFuncs::roundUp((this->container->GetHeight() * (originalPosition.y * 0.01f)), 1) - this->height * 0.5f; break;
		case POSITION_PERCENTAGE_RIGHT:
			this->position.y = StaticMenuFuncs::roundUp((this->container->GetHeight() * (originalPosition.y * 0.01f)), 1) - this->height; break;
		}
		break;
	}

	this->position.x += this->container->GetPos().x;
	this->position.y += this->container->GetPos().y;
}


Widget::~Widget()
{              
	delete container;
	delete widgetList;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		delete widgets[i];
	}
}

void Widget::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	bool insideThis = false;
	if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
	{
		insideThis = true;
	}

	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgets[i]->Input(insideThis, mousePos, inputManager, x, y);
	}
}

void Widget::UpdatePositionData(PositionType positionModeX, PositionType positionModeY, float x, float y, bool change)
{
	if (change)
	{
		LoadPositionData(ALIGNMENT_ABSOLUTE, glm::vec2(x, y), positionModeX, positionModeY);
		change = false;
	}
	else {
		LoadPositionData(this->alignment, this->originalPosition, this->positionModeX, this->positionModeY);
	}

	this->renderQuad.dest = Rect(this->GetPos().x, this->GetPos().y, this->GetWidth(), this->GetHeight());
	this->renderQuad.transform.x = (this->renderQuad.dest.centreX / (float)SCR_WIDTH) * 2.0f - 1.0f;
	this->renderQuad.transform.y = -((this->renderQuad.dest.centreY / (float)SCR_HEIGHT) * 2.0f - 1.0f);

	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgets[i]->UpdatePositionData(positionModeX, positionModeY, x, y, change);
	}
}

void Widget::ClearWidgets()
{
	int size = widgets.size();
	for (size_t i = 0; i < size; i++)
	{
		std::string widgetID = widgets[0]->GetID();
		(*widgetList).erase(widgetID);
		this->widgets.erase(widgets.begin());
	}
}