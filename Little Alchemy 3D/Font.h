#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "Constants.h"
#include "GUIQuad.h"

#include <iostream>
#include <map>
#include <fstream>

struct FontCharacter{
	GLuint  x, y, width, height, xadvance;
	GLint offsetx, offsety;
	FontCharacter(GLuint x, GLuint y, GLuint width, GLuint height, GLint offsetx, GLint offsety, GLuint xadvance);
	FontCharacter();
};

class Font
{
private:
	GLuint textureAtlas, padding[4], imgSize[2], lineHeight, charCount;
	GLint spacing[2];
	std::map<GLchar, FontCharacter> characters;

public:
	Font();
	~Font();

	void LoadFont(std::string name);
	void CreateVertices(std::string text, std::vector<GUIVertex>* verts, float spacingMod, glm::vec2* scale, glm::vec2* transform, Rect container, float *height, float* width);
	GLuint GetTextureAtlas();
};

