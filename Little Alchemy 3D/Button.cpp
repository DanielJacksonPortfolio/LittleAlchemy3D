#include "Button.h"

Button::Button(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, std::function<void()> vFunction, std::function<void(std::string)> sFunction, FunctionModes functionMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	LoadDefaultData(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY);
	Rect* dest = new Rect(this->position.x, this->position.y, this->width, this->height);
	this->renderQuad = GUIQuad(dest, colour);
	this->vFunction = vFunction;
	this->sFunction = sFunction;
	this->functionMode = functionMode;
	this->colour = colour;
	this->colourMode = colourMode;
}

void Button::Draw(Shader* imageShader, Shader* textShader, float x, float y)
{
	if (x != 0.0f || y != 0.0f)
	{
		storePosition = position;
		position.x += x;
		position.y += y;
	}
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		if (colourMode != TRANSPARENT) {
			renderQuad.Draw(imageShader, x, y);
		}
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->Draw(imageShader, textShader, x, y);
		}
	}
	if (x != 0.0f || y != 0.0f)
	{
		position = storePosition;
	}
}


Button::~Button()
{
}

void Button::HoverControl(bool hovering)
{
	if (hovering && !hovered)
	{
		hovered = true;
		colourMultiplier = 0.75f;
		renderQuad.colour = colour * colourMultiplier;
		for (size_t i = 0; i < widgets.size(); i++)
		{
			std::string type = typeid(*widgets[i]).name();
			type = type.substr(6);
			if (type == "GUIText")
			{
				widgets[i]->SetDrawMode(HOVER);
			}
		}
	}
	else if (!hovering && hovered)
	{
		hovered = false;
		colourMultiplier = 1.0f;
		renderQuad.colour = colour * colourMultiplier;
		for (size_t i = 0; i < widgets.size(); i++)
		{
			std::string type = typeid(*widgets[i]).name();
			type = type.substr(6);
			if (type == "GUIText")
			{
				widgets[i]->SetDrawMode(BASE);
			}

		}
	}
}


void Button::Input(bool aboveInside, glm::vec2 mousePos, InputManager* inputManager, float x, float y)
{
	if (x != 0.0f || y != 0.0f)
	{
		storePosition = position;
		position.x += x;
		position.y += y;
	}
	if (position.x > -width && position.x < SCR_WIDTH && position.y > -height && position.y < SCR_HEIGHT)
	{
		bool insideThis = false;
		if (aboveInside && mousePos.x > position.x && mousePos.x < position.x + width && mousePos.y > position.y && mousePos.y < position.y + height)
		{
			insideThis = true;
			HoverControl(true);

			if (inputManager->leftMouseState == 2) {
				inputManager->leftMouseState = 1;
				if (functionMode == VOID)
					std::invoke(vFunction);
				else if (functionMode == STRING)
					std::invoke(sFunction, this->ID);
			}
		}
		else
		{
			HoverControl(false);
		}
		for (size_t i = 0; i < widgets.size(); i++)
		{
			widgets[i]->Input(insideThis, mousePos, inputManager, x, y);
		}
	}
	if (x != 0.0f || y != 0.0f)
	{
		position = storePosition;
	}
}