#pragma once
#include "Page.h"
class List : public Page
{
private:
public:
	List(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	~List();
	void AutoAdjust() override;
};

