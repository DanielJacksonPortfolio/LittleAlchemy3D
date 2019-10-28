#pragma once
#include "Widget.h"
#include "GUIQuad.h"

class DisplayWidget : public Widget
{
protected:
	int textureWidth = -1;
	int textureHeight = -1;
	Rect sourceRect = Rect(0.0f,0.0f,1.0f,1.0f);
	Rect destRect;
public:
	DisplayWidget();
	~DisplayWidget();
	void UpdatePosition(Rect destRect);
	void UpdateSourceRect(Rect sourceRect);
	void Draw(Shader* imageShader, Shader* textShader, float x, float y);
	void UpdatePositionData(PositionType positionModeX, PositionType positionModeY, float x, float y, bool change) override;

};

