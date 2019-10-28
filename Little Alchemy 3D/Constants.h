#pragma once

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include "stb_image.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>

#define SCR_WIDTH 1600//1920
#define SCR_HEIGHT 900//1080
#define FULLSCREEN false

#define SHADOW_WIDTH 8192//2048//4096//8192//shadow texture width
#define SHADOW_HEIGHT 8192//2048//4096//8192//shadow texture height

#define PI 3.14159265359

#define DRAGGING_DISTANCE 3.0f
#define MAX_ITEM_COUNT 75

struct InputManager {
	int leftMouseState, rightMouseState;
	float scrollOffset;
	std::unordered_map<std::string, int> keyStates = { {"a",0}, {"b",0}, {"c",0}, {"d",0}, {"e",0}, {"f",0}, {"g",0}, {"h",0}, {"i",0}, {"j",0}, {"k",0}, {"l",0}, {"m",0}, {"n",0}, {"o",0},
												{"p",0}, {"q",0}, {"r",0}, {"s",0}, {"t",0}, {"u",0}, {"v",0}, {"w",0}, {"x",0}, {"y",0}, {"z",0}, {"0",0}, {"1",0}, {"2",0}, {"3",0},
												{"4",0}, {"5",0}, {"6",0}, {"7",0}, {"8",0}, {"9",0}, {"sp",0}, {"lsh",0}, {"es",0}, {"lct",0} , {"bksp",0} };//space, left shift, escape, left control
};

class ExtraMaths {
public:
	//write a vector to a file (not really maths)
	static void WriteVectorToFile(std::ofstream& file, glm::vec3* vector) {
		file << vector->x << " " << vector->y << " " << vector->z << "\n";
	}
	static void ReadVectorFromFile(std::ifstream& file, glm::vec3* vector) {
		file >> vector->x >> vector->y >> vector->z;
	}
	//lerp from point point1 to control to get point a and lerp from control to point 2 to get point b, and finally lerp from a to b
	static glm::vec3 QuadraticLerp(glm::vec3 point1, glm::vec3 control, glm::vec3 point2, float delta, bool normalize = false) {
		glm::vec3 p1c = ExtraMaths::LinearLerp(point1, control, delta, normalize);
		glm::vec3 pc2 = ExtraMaths::LinearLerp(control, point2, delta, normalize);
		return LinearLerp(p1c, pc2, delta, normalize);
	}		

	//linearly interpolate two vectors using simple interpolation (optional normalize)
	static glm::vec3 LinearLerp(glm::vec3 point1, glm::vec3 point2, float delta, bool normalize = false) {
		if (normalize) return glm::normalize(glm::vec3(point1.x + (point2.x - point1.x) * delta, point1.y + (point2.y - point1.y) * delta, point1.z + (point2.z - point1.z) * delta));
		else return glm::vec3(point1.x + (point2.x - point1.x) * delta, point1.y + (point2.y - point1.y) * delta, point1.z + (point2.z - point1.z) * delta);
	}

	//Euclidean distance betwteen two points
	static float Dist(glm::vec3 point1, glm::vec3 point2) {
		return sqrt(pow((point2.x - point1.x), 2) + pow((point2.y - point1.y), 2) + pow((point2.z - point1.z), 2));
	}

	//linearly interpolate an angle (errors accounted for)
	static float LerpAngle(float angle1, float angle2, float delta) {
		float diff = angle1 - angle2;
		float newAngle = 0.0f;
		if (abs(diff) > PI) {
			if (diff > 0) {
				float negitiveLeft = PI + angle2;
				float positiveLeft = PI - angle1;
				newAngle = angle1 + (negitiveLeft + positiveLeft) * delta;
			}
			else {
				float negitiveLeft = PI + angle1;
				float positiveLeft = PI - angle2;
				newAngle = angle1 - (negitiveLeft + positiveLeft) * delta;
			}
		}
		else {
			if (diff > 0) newAngle = angle1 - abs(diff) * delta;
			else newAngle = angle1 + abs(diff) * delta;
		}
		if (newAngle > PI) newAngle = -PI * 2 + newAngle;
		else if (newAngle < -PI) newAngle = PI * 2 + newAngle;
		
		return newAngle;
	}
	//get a vector representation of 3 euler angles
	static glm::vec3 GetVectorFromEuler(float yaw, float pitch, float roll) {
		return glm::vec3(-cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll),
			-sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll),
			cos(pitch) * sin(roll));
	}
	//get a rotation matrix from an axis and an angle
	static glm::mat4 GetRotationMatrix(float angle, glm::vec3 axis) {
		float s = sin(angle);
		float c = cos(angle);
		float oc = 1.0f - c;
		return glm::mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0f,
			oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0f,
			oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	//linearly interpolate between two vectors using a rotation matrix
	static glm::vec3 LerpVector(glm::vec3 vec1, glm::vec3 vec2, float delta) {
		float angle = acos(glm::clamp(glm::dot(vec1, vec2),-1.0f,1.0f));
		glm::vec3 axis = glm::normalize(glm::cross(vec1,vec2));
		glm::mat4 rotation = GetRotationMatrix(-angle * delta, axis);
		return glm::normalize(glm::vec3(rotation * glm::vec4(vec1, 1.0f)));
	}

	//lerp from point point1 to control to get point a and lerp from control to point 2 to get point b, and finally lerp from a to b
	static glm::vec3 QuadraticLerpVector(glm::vec3 vec1, glm::vec3 control, glm::vec3 vec2, float delta) {
		return ExtraMaths::LerpVector(ExtraMaths::LerpVector(vec1, control, delta), ExtraMaths::LerpVector(control, vec2, delta), delta);
	}

	//get the shortest distance between a line of infinite length and a point
	static float shortestDist(glm::vec3 lineStart, glm::vec3 lineEnd, glm::vec3 point)//infinite length
	{
		glm::vec3 startToEnd = lineEnd - lineStart;
		glm::vec3 startToPoint = point - lineStart;
		float area = ExtraMaths::Dist(glm::vec3(0.0f, 0.0f, 0.0f), glm::cross(startToEnd, startToPoint));
		return area / ExtraMaths::Dist(glm::vec3(0.0f, 0.0f, 0.0f), startToEnd);
	}

	//get the shortest distance between a line segment and a point
	static float shortestDistSegment(glm::vec3 lineStart, glm::vec3 lineEnd, glm::vec3 point) {
		const glm::vec3 l = lineEnd - lineStart;
		const float l2 = pow(l.x,2) + pow(l.y, 2) + pow(l.z, 2);
		if (l2 < 0.01f) return ExtraMaths::Dist(lineStart, point);   // lineStart == lineEnd
		const float t = glm::clamp(glm::dot(point - lineStart, lineEnd - lineStart) / l2, 0.0f, 1.0f);
		const glm::vec3 projection = lineStart + t * (lineEnd - lineStart);
		return ExtraMaths::Dist(point, projection);
	}

	//convert a 3d point into normalized device coordinates
	static glm::vec3 WorldToNDC(glm::vec3 point, glm::mat4* view, glm::mat4* projection) {
		glm::vec4 pointNdc = *projection * *view * glm::vec4(point, 1.0f);
		return glm::vec3(pointNdc.x / pointNdc.w, pointNdc.y / pointNdc.w, pointNdc.w);//perspective div
	}

	//get the height at a certain point
	static float BarryCentric(glm::vec3 polyPoint1, glm::vec3 polyPoint2, glm::vec3 polyPoint3, glm::vec2 checkPoint) {
		float det = (polyPoint2.z - polyPoint3.z) * (polyPoint1.x - polyPoint3.x) + (polyPoint3.x - polyPoint2.x) * (polyPoint1.z - polyPoint3.z);
		float l1 = ((polyPoint2.z - polyPoint3.z) * (checkPoint.x - polyPoint3.x) + (polyPoint3.x - polyPoint2.x) * (checkPoint.y - polyPoint3.z)) / det;
		float l2 = ((polyPoint3.z - polyPoint1.z) * (checkPoint.x - polyPoint3.x) + (polyPoint1.x - polyPoint3.x) * (checkPoint.y - polyPoint3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * polyPoint1.y + l2 * polyPoint2.y + l3 * polyPoint3.y;
	}

	//full barrycentric calculation, checks if a point is bounded by 3 other points
	static bool Barrycentric(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 checkPoint) {     
		glm::vec3 v0 = pointC - pointA;
		glm::vec3 v1 = pointB - pointA;
		glm::vec3 v2 = checkPoint - pointA;

		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);

		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0) && (v >= 0) && (u + v < 1);
	}

	//checks where a ray intersects a plane
	static glm::vec3 RayPlaneIntersect(glm::vec3 rayPoint, glm::vec3 rayDirection, glm::vec3 planeNormal, glm::vec3 planePoint) {
		glm::vec3 diff = rayPoint - planePoint;
		return (diff + planePoint) + rayDirection * (-glm::dot(diff, planeNormal) / glm::dot(rayDirection, planeNormal));
	}
	//gets the closest point on a plane to a point
	static glm::vec3 ClosestPointOnPlane(glm::vec3 planeNormal, glm::vec3 planePoint, glm::vec3 point) {
		glm::vec3 translationVector;
		float distance = ExtraMaths::SignedDistancePlanePoint(planeNormal, planePoint, point);
		distance *= -1.0f;
		translationVector = ExtraMaths::SetVectorLength(planeNormal, distance);
		return point + translationVector;
	}

	
	static float SignedDistancePlanePoint(glm::vec3 planeNormal, glm::vec3 planePoint, glm::vec3 point) {
		return glm::dot(planeNormal, (point - planePoint));
	}

	//sets the size of a vector to the given float value
	static glm::vec3 SetVectorLength(glm::vec3 vector, float size) {
		return glm::normalize(vector) * size;
	}
};

class TextureLoader {
public:
	//dynamically loads a texture using stbi
	static unsigned int LoadTexture(char const * path, GLint* outwidth = nullptr, GLint* outheight = nullptr)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (outwidth != nullptr) {
			*outwidth = width;
		}
		if (outheight != nullptr) {
			*outheight = height;
		}
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	//dynamically loads a cubemap using stbi
	static unsigned int LoadCubemap(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

	//dynamically loads a texture using stbi
	static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	static int MyLoadRaw(std::string path, int width, int height) {
		unsigned int ID;
		char* tempTextureData; int fileSize; std::ifstream inFile;
		inFile.open(path.c_str(), std::ios::binary);

		if (!inFile.good())
		{
			std::cerr << "Can't open texture file " << path.c_str() << std::endl;
			return false;
		}

		inFile.seekg(0, std::ios::end);
		fileSize = (int)inFile.tellg();
		tempTextureData = new char[fileSize];
		inFile.seekg(0, std::ios::beg);
		inFile.read(tempTextureData, fileSize);
		inFile.close();

		std::cout << path.c_str() << " loaded." << std::endl;

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete[] tempTextureData;
		return ID;
	}

	static int MyLoadTGA(std::string textureFileName) {
		unsigned int ID;
		char* tempHeaderData = new char[18];
		char* tempTextureData;
		int fileSize;
		char type, pixelDepth, mode;

		std::ifstream inFile;
		textureFileName = "res/" + textureFileName;
		inFile.open(textureFileName.c_str(), std::ios_base::binary);
		if (!inFile.good())
		{
			std::cerr << "Can't open texture file " << textureFileName.c_str() << std::endl;
			return -1;
		}

		inFile.seekg(0, std::ios::beg);
		inFile.read(tempHeaderData, 18);

		inFile.seekg(0, std::ios::end);
		fileSize = (int)inFile.tellg() - 18;
		tempTextureData = new char[fileSize];
		inFile.seekg(18, std::ios::beg);
		inFile.read(tempTextureData, fileSize);
		inFile.close(); 

		type = tempHeaderData[2]; 
		unsigned int _width = ((unsigned int)tempHeaderData[13] << 8u) + (unsigned int)tempHeaderData[12];
		unsigned int _height = ((unsigned int)tempHeaderData[15] << 8u) + (unsigned int)tempHeaderData[14];

		pixelDepth = tempHeaderData[16];

		bool flipped = false;
		if ((int)((tempHeaderData[11] << 8) + tempHeaderData[10]) == 0)
			flipped = true;


		if ((int)type == 2)
		{
			std::cout << textureFileName.c_str() << " loaded." << std::endl;

			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);

			mode = pixelDepth / 8;

			if (mode == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData);
			}
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		delete[] tempHeaderData;
		delete[] tempTextureData;

		return ID;
	}
};
//
//class HDRtoCubemap {
//public:
//	static int GetTextureID(std::string path, Shader* ETCShader) {
//	// pbr: setup framebuffer
//	// ----------------------
//		unsigned int captureFBO;
//		unsigned int captureRBO;
//		glGenFramebuffers(1, &captureFBO);
//		glGenRenderbuffers(1, &captureRBO);
//
//		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
//		// pbr: load the HDR environment map
//	// ---------------------------------
//		stbi_set_flip_vertically_on_load(true);
//		int width, height, nrComponents;
//		float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);//"res/images/hdr/newport_loft.hdr"
//		unsigned int hdrTexture;
//		if (data) {
//			glGenTextures(1, &hdrTexture);
//			glBindTexture(GL_TEXTURE_2D, hdrTexture);
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float
//
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//			stbi_image_free(data);
//		}
//		else
//		{
//			std::cout << "Failed to load HDR image." << std::endl;
//		}
//
//		// pbr: setup cubemap to render to and attach to framebuffer
//		// ---------------------------------------------------------
//		unsigned int envCubemap;
//		glGenTextures(1, &envCubemap);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//		for (unsigned int i = 0; i < 6; ++i)
//		{
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
//		}
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
//		// ----------------------------------------------------------------------------------------------
//		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
//		glm::mat4 captureViews[] =
//		{
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
//		};
//
//		// pbr: convert HDR equirectangular environment map to cubemap equivalent
//		// ----------------------------------------------------------------------
//		ETCShader->use();
//		ETCShader->setInt("equirectangularMap", 0);
//		ETCShader->setMat4("projection", captureProjection);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, hdrTexture);
//
//		glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
//		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//		for (unsigned int i = 0; i < 6; ++i)
//		{
//			ETCShader->setMat4("view", captureViews[i]);
//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_Colour_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
//			glClear(GL_Colour_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//			renderCube();
//		}
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	}
//};