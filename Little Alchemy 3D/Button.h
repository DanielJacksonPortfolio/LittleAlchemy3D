#pragma once
#include "Widget.h"
#include "Text.h"
#include <functional>
class Button : public Widget
{
private:
	glm::vec3 colour;
	ColourMode colourMode = TRANSPARENT;
	std::function<void()> vFunction = nullptr;
	std::function<void(std::string)> sFunction = nullptr;
	FunctionModes functionMode = VOID;
	float colourMultiplier = 1.0f;
	bool hovered = false;
public:
	Button(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, std::function<void()> vFunction, std::function<void(std::string)> sFunction, FunctionModes functionMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	~Button();
	void Draw(Shader* imageShader, Shader* textShader, float x, float y) override;
	void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y) override;
	void HoverControl(bool hovering);
};