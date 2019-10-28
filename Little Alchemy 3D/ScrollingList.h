#pragma once
#include "Page.h"
#include "List.h"
class ScrollingList : public Page
{
private:
	std::vector<List*> lists = {};
	int pageCount = 0;
	int showList = 0;
	int displayWidgetAmount = 0;
	ScrollMode scrollMode = LIST;
	float viewY = 0.0f;
	float totalHeight = 0.0f;
public:
	ScrollingList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, ScrollMode scrollMode, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	~ScrollingList();
	void AutoAdjust() override;
	void Draw(Shader* imageShader, Shader* textShader, float x, float y) override;
	void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y) override;
	void SearchWidgets(std::string searchText) override;
};

