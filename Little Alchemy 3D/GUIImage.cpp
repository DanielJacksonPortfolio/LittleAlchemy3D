#include "GUIImage.h"


GUIImage::GUIImage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, Rect sourceRect, GUITexture* texture, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY,ImageLoadModes imageLoadMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	this->imageLoadMode = imageLoadMode;
	
	switch (this->imageLoadMode)
	{
	case STRETCH:
		UpdateSourceRect(Rect(0.0f, 0.0f, texture->width, texture->height));
		break;
	case ORIGINAL:
		this->width = texture->width;
		this->height = texture->height;
		UpdateSourceRect(Rect(0.0f, 0.0f, texture->width, texture->height));
		break;
	case EXACT:
		UpdateSourceRect(sourceRect);
		break;
	}
	//UpdatePositionData(this->alignment,this->positionModeX,this->positionModeY,0.0f,0.0f);
	UpdatePosition(Rect(this->position.x,this->position.y,this->width,this->height));
	renderQuad = GUIQuad(&this->destRect, glm::vec3(1.0f, 1.0f, 1.0f), &this->sourceRect, texture);
}

GUIImage::~GUIImage()
{
}