#include "List.h"

List::List(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	Rect* dest = new Rect(this->position.x, this->position.y, this->width, this->height);
	this->renderQuad = GUIQuad(dest, colour);
	this->colour = colour;
	this->colourMode = colourMode;
}


List::~List()
{
}

void List::AutoAdjust()
{
	int widgetH = 0;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgetH += widgets[i]->GetHeight();
	}
	int spare = this->height - widgetH;
	float gap = spare / (widgets.size()+1);
	float pos = gap;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgets[i]->UpdatePositionData(POSITION_PERCENTAGE_CENTRE,POSITION_ABSOLUTE,50,pos,true);
		pos += gap;
		pos += widgets[i]->GetHeight();
	}
}