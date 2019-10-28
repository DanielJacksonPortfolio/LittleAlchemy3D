#pragma once
#include "MenuConstants.h"
#include "Constants.h"
#include "Shader.h"
#include "Font.h"

//#include "Font.h"
#include <unordered_map>
#include <functional>

class Widget
{
protected:
	std::string ID = "NULL";
	int width = SCR_WIDTH, height = SCR_HEIGHT;
	glm::vec2 originalPosition = glm::vec2(0.0f, 0.0f);
	glm::vec2 position = glm::vec2(0.0f, 0.0f), storePosition = glm::vec2(0.0f,0.0f);
	Widget* container = nullptr;
	AlignmentType alignment = ALIGNMENT_ABSOLUTE;
	DimensionType widthMode = DIMENSION_ABSOLUTE, heightMode = DIMENSION_ABSOLUTE;
	PositionType positionModeX = POSITION_ABSOLUTE, positionModeY = POSITION_ABSOLUTE;
	glm::vec3 colour = glm::vec3(0.0f, 0.0f, 0.0f);
	ColourMode colourMode = FILLED;
	std::vector<Widget*> widgets = {}, displayWidgets = {};
	std::unordered_map<std::string, Widget*>* widgetList = nullptr;

public:
	GUIQuad renderQuad;

	Widget();
	virtual ~Widget();
	
	void AddPage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	void AddList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode , DimensionType heightMode , AlignmentType alignment, glm::vec2 position, PositionType positionModeX , PositionType positionModeY , glm::vec3 colour , ColourMode colourMode );
	void AddScrollingList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, ScrollMode scrollMode, int width, int height, DimensionType widthMode , DimensionType heightMode , AlignmentType alignment, glm::vec2 position, PositionType positionModeX , PositionType positionModeY , glm::vec3 colour , ColourMode colourMode );
	void AddText(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, std::string textString, Font* font, float targetHeight, int width, int height, TextDisplayModes displayMode , DimensionType widthMode , DimensionType heightMode , AlignmentType alignment , glm::vec2 position, PositionType positionModeX , PositionType positionModeY , glm::vec3 baseColour, glm::vec3 hoverColour, TextRenderModes renderMode);
	void AddImage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, Rect sourceRect, GUITexture* texture, DimensionType widthMode , DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, ImageLoadModes imageLoadMode);
	void AddButton(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, std::function<void()> vFunction, std::function<void(std::string)> sFunction, FunctionModes functionMode, DimensionType widthMode , DimensionType heightMode , AlignmentType alignment, glm::vec2 position, PositionType positionModeX , PositionType positionModeY, glm::vec3 colour, ColourMode colourMode);
	void AddGrid(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode, ScrollMode scrollMode, int rowCount, RowModes rowMode);
	void AddSearch(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, int maxLength, Widget* searchContainer, glm::vec3 colour, ColourMode colourMode);

	virtual void Update() {}
	virtual void Draw(Shader* imageShader, Shader* textShader, float x = 0.0f, float y = 0.0f) {}
	virtual void Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x = 0.0f, float y = 0.0f);
	
	void LoadDefaultData(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY);
	void LoadDimensionData(int width, int height, DimensionType widthMode, DimensionType heightMode);
	void LoadPositionData(AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY);
	void LoadInformation(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container);

	void SetContainer(Widget* container) { this->container = container; LoadPositionData(this->alignment, this->originalPosition, this->positionModeX, this->positionModeY); }

	void SetAllWidgets() { this->displayWidgets = this->widgets; AutoAdjust(); }
	void ShowAllWidgets() { this->displayWidgets = this->widgets; }
	void SetColour(glm::vec3 colour) { this->colour = colour; }
	void ClearWidgets();

	std::vector<Widget*>* GetWidgets() { return &widgets; }
	std::vector<Widget*>* GetDisplayWidgets() { return &displayWidgets; }
	AlignmentType GetAlignment() { return alignment; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	glm::vec2 GetPos() { return position; }
	std::string const GetID() { return ID; }
	void virtual UpdatePositionData(PositionType positionModeX, PositionType positionModeY, float x, float y, bool change);

	glm::vec3 virtual const GetColour() { return { 0,0,0 }; }
	void virtual SetDrawMode(DrawMode drawMode) {};
	void virtual UpdateColour(glm::vec3 colour, DrawMode drawMode) {};
	void virtual AutoAdjust() {};
	void virtual SearchWidgets(std::string searchText) {};
	virtual void UpdateText(std::string newText) {}
	bool virtual GetActive() { return false; }

};
