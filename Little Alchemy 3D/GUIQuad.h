#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "Constants.h"
#include "Shader.h"

#include <vector>

struct GUITexture {
	GLuint data;
	GLint width, height;
	GUITexture(std::string path) {
		data = TextureLoader::LoadTexture(path.c_str(), &width, &height);
	}
};

struct GUIVertex {
	glm::vec2 position;
	glm::vec2 texCoords;
	GUIVertex(glm::vec2 position, glm::vec2 texCoords) {
		this->position = position; this->texCoords = texCoords;
	}
	GUIVertex(GLfloat vx, GLfloat vy, GLfloat tx, GLfloat ty) {
		this->position.x = vx; this->position.y = vy; this->texCoords.x = tx; this->texCoords.y = ty;
	}
	GUIVertex() {}
};

struct Rect {
public:
	float left, right, top, bot, width, height, centreX, centreY;
	Rect(float left, float top, float width, float height) {
		this->left = left; this->right = left + width;
		this->top = top; this->bot = top + height;
		this->width = width; this->height = height;
		this->centreX = left + width * 0.5f;
		this->centreY = top + height * 0.5f;
	}
	Rect() {
		this->left = 0.0f;
		this->right = 0.0f;
		this->top = 0.0f;
		this->bot = 0.0f;
		this->centreX = 0.0f;
		this->centreY = 0.0f;
		this->width = 0.0f;
		this->height = 0.0f;
	}
};

class GUIQuad
{
private:
	GLuint VAO, VBO;
	GUITexture* texture;
	std::vector<GUIVertex> vertices;

public:
	glm::vec2 scale;
	glm::vec2 transform;
	Rect dest, source;
	glm::vec3 colour;
	bool hasTexture;

	GUIQuad(Rect* dest, glm::vec3 colour, Rect* source = nullptr, GUITexture* texture = nullptr);
	GUIQuad();
	~GUIQuad();
	void SetUp();
	void CreateVertices();
	void Draw(Shader* GUIShader, float x, float y);
	void UpdateScaleAndTransform();
};

