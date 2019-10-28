#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "Constants.h"
#include "Font.h"
#include "Shader.h"

class Text
{
private:
	glm::vec3 colour;
	std::string text;
	Font* font;
	GLuint VBO, VAO;
	bool dropShadow = false;
	
	float spacingMod;
	
public:
	Rect containerRect;
	glm::vec2 scale;
	glm::vec2 transform;
	float width, height;
	std::vector<GUIVertex> verts;

	Text(std::string text, Font* font, Rect containerRect, glm::vec3 colour, float spacingMod, float targetHeight = -1.0f);
	~Text();

	//setup
	void Setup();

	//update
	void SetText(std::string text);
	void UpdateQuads();

	//draw
	void Draw(Shader* shader, float x = 0.0f, float y = 0.0f);
	void UpdatePositionAndScale(Rect* destRect);
	std::string GetText() { return text; };
	
};

