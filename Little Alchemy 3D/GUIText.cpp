#include "GUIText.h"

GUIText::GUIText(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, std::string textString, Font* font, float targetHeight, int width, int height, TextDisplayModes displayMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 baseColour, glm::vec3 hoverColour, TextRenderModes renderMode)
{
	LoadInformation(widgetList, ID, container);

	this->displayMode = displayMode;

	//parse in font
	this->textString = textString;
	this->baseColour = baseColour;
	this->hoverColour = hoverColour;
	this->renderMode = renderMode;

	LoadDimensionData(width, height, widthMode, heightMode);

	switch (this->displayMode)
	{
	case CONTAINER:
		this->destRect = Rect(this->container->GetPos().x, this->container->GetPos().y, this->container->GetWidth(), this->container->GetHeight());
		text = new Text(textString, font, this->destRect, baseColour, 0.8f, targetHeight);
		break;
	case DIMENSION_BASED:
		text = new Text(textString, font, this->destRect, baseColour, 0.8f, targetHeight);
		break;
	}

	this->width = this->text->width;
	this->height = this->text->height;

	LoadPositionData(alignment, position, positionModeX, positionModeY);

	UpdatePosition(Rect(this->position.x, this->position.y, this->width, this->height));
	
	text->UpdatePositionAndScale(&this->destRect);

	//std::cout << "dest: " << this->destRect.centreX << " " << this->destRect.centreY << " transform: " << this->text->transform.x << " " << this->text->transform.y << " scale: " << this->text->scale.x << " " << this->text->scale.y << std::endl;
	
	//case DIMENSION_BASED:
	//	//put auto dimension code
	//	break;
	//case CUTOFF:
	//	if (textureWidth > this->width)
	//	{
	//		if (textureHeight < this->height)
	//		{
	//			this->height = textureHeight;
	//		}	
	//		UpdateSourceRect(Rect(0,0,this->width,this->height));

	//	}
	//	else
	//	{
	//		this->width = textureWidth;

	//		if (textureHeight > this->height)
	//		{
	//			UpdateSourceRect(Rect(0,0,this->width,this->height));
	//		}
	//		else
	//		{
	//			this->height = textureHeight;
	//		}

	//	}
	//	UpdatePosition(Rect(this->position.x,this->position.y,this->width,this->height));
	//	break;
	//case DISABLE_RESTRAINTS:
	//	this->width =  textureWidth;
	//	this->height = textureHeight;
	//	break;
	//}
}

GUIText::~GUIText()
{
	delete container;
	delete widgetList;
}

void GUIText::UpdateColour(glm::vec3 colour,DrawMode drawMode)
{
	if (drawMode == HOVER)
		this->hoverColour = colour;
	else
		this->baseColour = colour;
}

void GUIText::SetDrawMode(DrawMode drawMode)
{
	this->drawMode = drawMode;
}

void GUIText::Draw(Shader* imageShader, Shader* textShader, float x, float y) {
	text->Draw(textShader, x, y);
}

void GUIText::UpdatePositionData(PositionType positionModeX, PositionType positionModeY, float x, float y, bool change)
{
	if (change)
	{
		LoadPositionData(ALIGNMENT_ABSOLUTE, glm::vec2(x, y), positionModeX, positionModeY);
		change = false;
	}
	else {
		LoadPositionData(this->alignment, this->originalPosition, this->positionModeX, this->positionModeY);
	}

	UpdatePosition(Rect(this->position.x, this->position.y, this->width, this->height));

	this->text->containerRect = Rect(this->GetPos().x, this->GetPos().y, this->GetWidth(), this->GetHeight());
	this->text->transform.x = (this->text->containerRect.centreX / (float)SCR_WIDTH) * 2.0f - 1.0f;
	this->text->transform.y = -((this->text->containerRect.centreY / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}

void GUIText::UpdateText(std::string newText)
{
	this->textString = newText;
	this->text->SetText(this->textString);

	text->UpdateQuads();
	//LoadPositionData(alignment, position, positionModeX, positionModeY);

	//UpdatePosition(Rect(this->position.x, this->position.y, this->width, this->height));
	/*switch (this->displayMode)
	{
	case CONTAINER:
		this->text->containerRect = this->destRect;
		text->UpdateQuads();
		break;
	case DIMENSION_BASED:
		this->text->containerRect = this->destRect;
		text->UpdateQuads();
		break;
	}

	this->width = this->text->width;
	this->height = this->text->height;

	LoadPositionData(alignment, position, positionModeX, positionModeY);

	UpdatePosition(Rect(this->position.x, this->position.y, this->width, this->height));
*/
	//text->UpdatePositionAndScale(&this->destRect);
}
