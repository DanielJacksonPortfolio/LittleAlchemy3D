#pragma once
#include "DisplayWidget.h"
#include "Font.h"
#include "Text.h"

class GUIText : public DisplayWidget
{
private:
	std::string textString;
	Font* font = nullptr;
	glm::vec3 baseColour;
	glm::vec3 hoverColour;
	TextRenderModes renderMode = SOLID;
	TextDisplayModes displayMode = DIMENSION_BASED;
	DrawMode drawMode = BASE;
	Text* text = nullptr;
public:
	GUIText(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, std::string textString, Font* font, float targetHeight, int width, int height, TextDisplayModes displayMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 baseColour, glm::vec3 hoverColour,TextRenderModes renderMode);
	~GUIText();
	void UpdateColour(glm::vec3 colour,DrawMode drawMode) override;
	void SetDrawMode(DrawMode drawMode) override;
	void LoadText(std::string text, Font* font, glm::vec3 colour, int renderMode);
	void Draw(Shader* imageShader, Shader* textShader, float x, float y) override;
	glm::vec3 virtual const GetColour(DrawMode drawMode) { if (drawMode == BASE) return baseColour; else return hoverColour; }
	void UpdatePositionData(PositionType positionModeX, PositionType positionModeY, float x, float y, bool change) override;
	void UpdateText(std::string newText) override;
};

