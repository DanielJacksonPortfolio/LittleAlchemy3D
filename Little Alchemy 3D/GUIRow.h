#pragma once
#include "Page.h"
#include "MenuConstants.h"

class GUIRow : public Page
{
protected:
	int totalWidth = 0;
public:
	GUIRow(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode ColourMode);
	~GUIRow();
	void AutoAdjust() override;
	void Draw(Shader* imageShader, Shader* textShader, float x = 0.0f, float y = 0.0f) override;
	void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x = 0.0f, float y = 0.0f);
	void SearchWidgets(std::string searchText) override;
};

