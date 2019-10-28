#include "GUIQuad.h"



GUIQuad::GUIQuad(Rect* dest, glm::vec3 colour, Rect* source, GUITexture* texture)
{
	this->texture = texture;
	this->dest = *dest;
	this->colour = colour;
	if (texture != nullptr) {
		this->hasTexture = true;
	}
	else this->hasTexture = false;
	if (source != nullptr) {
		this->source = *source;
	}
	CreateVertices();
	SetUp();
	UpdateScaleAndTransform();
}

GUIQuad::GUIQuad()
{
}


GUIQuad::~GUIQuad()
{
}

void GUIQuad::CreateVertices() {
	float normalizedTextureLeft = hasTexture ? source.left / (float)this->texture->width : 0.0f;
	float normalizedTextureRight = hasTexture ? source.right / (float)this->texture->width : 1.0f;
	float normalizedTextureTop = hasTexture ? source.top / (float)this->texture->height : 0.0f;
	float normalizedTextureBot = hasTexture ? source.bot / (float)this->texture->height : 1.0f;
	vertices.push_back(GUIVertex(-1.0, 1.0, normalizedTextureLeft, normalizedTextureTop));
	vertices.push_back(GUIVertex(1.0, 1.0, normalizedTextureRight, normalizedTextureTop));
	vertices.push_back(GUIVertex(-1.0, -1.0, normalizedTextureLeft, normalizedTextureBot));
	vertices.push_back(GUIVertex(1.0, -1.0, normalizedTextureRight, normalizedTextureBot));
}

void GUIQuad::UpdateScaleAndTransform() {
	this->scale.x = dest.width / (float)SCR_WIDTH;
	this->scale.y = dest.height / (float)SCR_HEIGHT;

	this->transform.x = (dest.centreX / (float)SCR_WIDTH) * 2.0f - 1.0f;
	this->transform.y = -((dest.centreY / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}

void GUIQuad::SetUp() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GUIVertex), &vertices[0], GL_STATIC_DRAW);

	// vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);
	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)offsetof(GUIVertex, texCoords));

	glBindVertexArray(0);
}

void  GUIQuad::Draw(Shader* GUIShader, float x, float y) {

	GUIShader->use();
	GUIShader->setBool("useTexture", hasTexture);
	GUIShader->setVec3("colour", colour);

	GUIShader->setVec2("scale", scale);
	if (x != 0.0f || y != 0.0f) {
		GUIShader->setVec2("transform", transform + glm::vec2((x / (float)(SCR_WIDTH)) * 2.0f, (y / (float)(SCR_HEIGHT)) * -2.0f));
	}
	else {
		GUIShader->setVec2("transform", transform);
	}
	
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);
	glEnable(GL_CLIP_DISTANCE2);
	glEnable(GL_CLIP_DISTANCE3);

	if (hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->data);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}