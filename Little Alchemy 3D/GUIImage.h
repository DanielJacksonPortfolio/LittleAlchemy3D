#pragma once
#include "DisplayWidget.h"
class GUIImage : public DisplayWidget
{
private:
	ImageLoadModes imageLoadMode = ORIGINAL;
public:
	GUIImage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, Rect sourceRect, GUITexture* texture, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, ImageLoadModes imageLoadMode);
	~GUIImage();
};

