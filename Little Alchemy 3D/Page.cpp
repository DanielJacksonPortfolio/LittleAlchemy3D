#include "Page.h"

Page::Page(std::unordered_map<std::string, Widget*> * widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	Rect* dest = new Rect(this->position.x, this->position.y, this->width, this->height);
	this->renderQuad = GUIQuad(dest,colour);
	this->colour = colour;
	this->colourMode = colourMode;
}

void Page::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (colourMode != TRANSPARENT) {
		renderQuad.Draw(imageShader, x, y);
	}

	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgets[i]->Draw(imageShader, textShader, x, y);
	}
}


Page::~Page()
{
}
