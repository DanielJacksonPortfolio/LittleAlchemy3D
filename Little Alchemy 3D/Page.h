#pragma once
#include "Widget.h"
#include "GUIQuad.h"
class Page : public Widget
{
public:
	Page() {}
	Page(std::unordered_map<std::string, Widget*> * widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	~Page();
	void Draw(Shader* imageShader, Shader* textShader, float x, float y) override;
};

