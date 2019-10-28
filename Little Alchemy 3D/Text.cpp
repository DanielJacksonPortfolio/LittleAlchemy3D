#include "Text.h"



Text::Text(std::string text, Font* font, Rect containerRect, glm::vec3 colour, float spacingMod, float targetHeight) {
	this->text = text;
	this->font = font;
	this->containerRect = containerRect;
	this->height = targetHeight;
	this->colour = colour;
	this->spacingMod = spacingMod;
	this->Setup();
}

Text::~Text()
{
}

void Text::Setup() {
	font->CreateVertices(this->text, &this->verts, this->spacingMod, &this->scale, &this->transform, this->containerRect, &this->height, &this->width);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GUIVertex), &verts[0], GL_STATIC_DRAW);

	// vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);
	// vertex texcoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)offsetof(GUIVertex, texCoords));

	glBindVertexArray(0);
}

void Text::UpdateQuads() {
	this->verts.clear();
	font->CreateVertices(this->text, &this->verts, this->spacingMod, &this->scale, &this->transform, this->containerRect, &this->height, &this->width);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GUIVertex), &verts[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Text::SetText(std::string text) {
	this->text = text;
}

void Text::Draw(Shader* shader, float x, float y) {
	shader->use();

	shader->setVec3("textColour", this->colour);

	shader->setVec2("scale", scale);
	if (x != 0.0f || y != 0.0f) {
		shader->setVec2("transform", transform + glm::vec2((x / (float)(SCR_WIDTH)) * 2.0f, (y / (float)(SCR_HEIGHT)) * -2.0f));
	}
	else {
		shader->setVec2("transform", transform);
	}
	

	shader->setBool("dropShadow", dropShadow);
	shader->setFloat("textHeight", height);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->font->GetTextureAtlas());

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->verts.size());

	glBindVertexArray(0);
}

void Text::UpdatePositionAndScale(Rect * destRect) {
	scale.x = (this->width / (float)SCR_WIDTH);
	scale.y = (this->height / (float)SCR_HEIGHT);

	transform.x = (destRect->centreX / (float)SCR_WIDTH) * 2.0f - 1.0f;
	transform.y = -((destRect->centreY / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}