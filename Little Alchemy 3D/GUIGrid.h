#pragma once
#include "Page.h"
#include "GUIRow.h"
#include "GUISearch.h"
#include "ScrollingList.h"

class GUIGrid : public Page
{
private:
	int rowCount = 0;
	std::vector<GUIRow*> rows = {};
	ScrollingList* list = nullptr;
	Widget* search = nullptr;
	RowModes rowMode = WIDGET;
public:
	GUIGrid(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, ScrollMode scrollMode, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode, int rowCount, RowModes rowMode);
	~GUIGrid();
	void AutoAdjust() override;
	void Draw(Shader* imageShader, Shader* textShader, float x = 0.0f, float y = 0.0f) override;
	void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x = 0.0f, float y = 0.0f) override;
	void SearchWidgets(std::string searchText) override;
};

