#pragma once
#include "Widget.h"
#include "GUIText.h"
#include "Constants.h"

class GUISearch : public Widget
{
private:
	float colourMultiplier = 1.0f;
	bool hovered = false, active = false;

	Widget* searchContainer = nullptr;
	std::string searchText = "", displayText = "";
	int maxLength = 0;
public:
	GUISearch(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int maxLength, int width, int height, Widget* searchContainer, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	~GUISearch();
	void Draw(Shader* imageShader, Shader* textShader, float x = 0.0f, float y = 0.0f) override;
	void HoverControl(bool hovering);
	void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x = 0.0f, float y = 0.0f) override;
	bool GetActive() override { return active; }
};

