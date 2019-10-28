#include "DisplayWidget.h"

DisplayWidget::DisplayWidget()
{
}

DisplayWidget::~DisplayWidget()
{
	delete container;
	delete widgetList;
	for (size_t i = 0; i < widgets.size(); i++)
	{
		delete widgets[i];
	}
}

void DisplayWidget::UpdatePosition(Rect destRect)
{
	this->destRect = destRect;
}

void DisplayWidget::UpdateSourceRect(Rect sourceRect)
{
	this->sourceRect = sourceRect;
}

void DisplayWidget::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	renderQuad.Draw(imageShader, x, y);
}

void DisplayWidget::UpdatePositionData(PositionType positionModeX, PositionType positionModeY,float x, float y, bool change)
{
	if (change)
	{
		LoadPositionData(ALIGNMENT_ABSOLUTE, glm::vec2(x, y), positionModeX, positionModeY);
		change = false;
	}
	else {
		LoadPositionData(this->alignment, this->originalPosition, this->positionModeX, this->positionModeY);
	}

	this->renderQuad.dest = Rect(this->GetPos().x, this->GetPos().y, this->GetWidth(), this->GetHeight());
	this->renderQuad.transform.x = (this->renderQuad.dest.centreX / (float)SCR_WIDTH) * 2.0f - 1.0f;
	this->renderQuad.transform.y = -((this->renderQuad.dest.centreY / (float)SCR_HEIGHT) * 2.0f - 1.0f);

	UpdatePosition(Rect(this->position.x,this->position.y,this->width,this->height));
}