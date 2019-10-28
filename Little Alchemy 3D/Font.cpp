#include "Font.h"



Font::Font()
{
}


Font::~Font()
{
}

FontCharacter::FontCharacter(GLuint x, GLuint y, GLuint width, GLuint height, GLint offsetx, GLint offsety, GLuint xadvance) {
	this->x = x; this->y = y; this->width = width; this->height = height; this->offsetx = offsetx; this->offsety = offsety, this->xadvance = xadvance;
}

FontCharacter::FontCharacter() {

}

void Font::LoadFont(std::string name) {
	std::string texturePath = "res/images/font images/" + name + ".png";
	textureAtlas = TextureLoader::LoadTexture(texturePath.c_str());
	std::string fontFilePath = "res/fonts/" + name + ".fnt";
	std::ifstream inFile(fontFilePath.c_str());
	std::string buffer;
	if (inFile.is_open()) {
		//load padding
		inFile >> buffer;
		for (int i = 0; i < 4; i++) {
			inFile >> padding[i];
		}
		//load spacing
		inFile >> buffer;
		for (int i = 0; i < 2; i++) {
			inFile >> spacing[i];
		}
		//load scale
		inFile >> buffer;
		inFile >> imgSize[0];
		inFile >> buffer;
		inFile >> imgSize[1];
		//load line height
		inFile >> buffer;
		inFile >> lineHeight;
		//load character count
		inFile >> buffer;
		inFile >> charCount;
		//load character data
		for (int i = 0; i < charCount; i++) {
			GLuint  id, x, y, width, height, xadvance;
			GLint offsetx, offsety;
			//load id
			inFile >> buffer;
			inFile >> id;
			//load position
			inFile >> buffer;
			inFile >> x;
			inFile >> buffer;
			inFile >> y;
			//load width and height
			inFile >> buffer;
			inFile >> width;
			inFile >> buffer;
			inFile >> height;
			//load x and y offset
			inFile >> buffer;
			inFile >> offsetx;
			inFile >> buffer;
			inFile >> offsety;
			//load x advance
			inFile >> buffer;
			inFile >> xadvance;
			FontCharacter character = FontCharacter(x, y, width, height, offsetx, offsety, xadvance);
			characters.insert(std::pair<GLchar, FontCharacter>((char)id, character));
		}
	}
}

void Font::CreateVertices(std::string text, std::vector<GUIVertex>* verts, float spacingMod, glm::vec2 * scale, glm::vec2* transform, Rect containerRect, float *height, float* width) {
	float posX = 0.0f;

	float leftMostVertex = 0.0f;
	float rightMostVertex = 0.0f;
	float topMostVertex = -FLT_MAX;
	float botMostVertex = FLT_MAX;

	FontCharacter firstCharacter = characters[text.at(0)];
	leftMostVertex = firstCharacter.offsetx;

	for (int i = 0; i < text.length(); i++) {
		FontCharacter character = characters[text.at(i)];
		unsigned int currentVertexIndex = verts->size();

		verts->push_back(GUIVertex(posX + character.offsetx, -(GLint)character.offsety, (float)character.x / (float)imgSize[0], (float)character.y / (float)imgSize[1]));
		verts->push_back(GUIVertex(posX + character.offsetx, (-(GLint)character.offsety - (GLint)character.height), (float)character.x / (float)imgSize[0], (float)(character.y + character.height) / (float)imgSize[1]));
		verts->push_back(GUIVertex(posX + (character.offsetx + character.width), -(GLint)character.offsety, (float)(character.x + character.width) / (float)imgSize[0], (float)character.y / (float)imgSize[1]));

		verts->push_back(GUIVertex(posX + (character.offsetx + character.width), -(GLint)character.offsety, (float)(character.x + character.width) / (float)imgSize[0], (float)character.y / (float)imgSize[1]));
		verts->push_back(GUIVertex(posX + character.offsetx,(-(GLint)character.offsety - (GLint)character.height), (float)character.x / (float)imgSize[0], (float)(character.y + character.height) / (float)imgSize[1]));
		verts->push_back(GUIVertex(posX + (character.offsetx + character.width), (-(GLint)character.offsety - (GLint)character.height), (float)(character.x + character.width) / (float)imgSize[0], (float)(character.y + character.height) / (float)imgSize[1]));

		posX += character.xadvance * spacingMod;

		for (int j = 0; j < 6; j++) {
			if (verts->at(currentVertexIndex + j).position.y > topMostVertex) {
				topMostVertex = verts->at(currentVertexIndex + j).position.y;
			}
			else if (verts->at(currentVertexIndex + j).position.y < botMostVertex) {
				botMostVertex = verts->at(currentVertexIndex + j).position.y;
			}
		}
	}

	FontCharacter lastCharacter = characters[text.back()];
	rightMostVertex = posX;

	float textWidth = rightMostVertex - leftMostVertex;
	float textheight = topMostVertex - botMostVertex;

	for (int i = 0; i < verts->size(); i++) {
		verts->at(i).position.x = ((verts->at(i).position.x - leftMostVertex) / textWidth) * 2.0f - 1.0f;
		verts->at(i).position.y = ((verts->at(i).position.y - botMostVertex) / textheight) * 2.0f - 1.0f;
	}

	if (*height > 0.0f) {
		//target height specified
		float ratio = textWidth / textheight;
		*width = (*height) * ratio;
	}
	else {
		//use container
		float ratio = textWidth / textheight;
		float containerRatio = containerRect.width / containerRect.height;
		if (ratio > containerRatio) {//longer than container
			*width = containerRect.width;
			*height = (*width) / ratio;
		}
		else {//shorter
			*height = containerRect.height;
			*width = (*height) * ratio;
		}
	}

	////normalised screen scale
	//float ratio = textWidth / textheight;
	//float containerRatio = containerRect.width / containerRect.height;
	//if (ratio > containerRatio) {//longer than screen
	//	scale->x = 1.0f;
	//	scale->y = ((containerRect.width / textWidth) * textheight) / containerRect.height;
	//}
	//else {//shorter
	//	scale->x = ((containerRect.height / textheight) * textWidth) / containerRect.width;
	//	scale->y = 1.0f;
	//}
}

GLuint Font::GetTextureAtlas() {
	return textureAtlas;
}
