#pragma once

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	Vertex(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat tx, GLfloat ty) {
		Position.x = vx; Position.y = vy; Position.z = vz; Normal.x = nx; Normal.y = ny; Normal.z = nz; TexCoords.x = tx; TexCoords.y = ty;
	}
	Vertex(glm::vec3 position, glm::vec3 normal, GLfloat tx, GLfloat ty) {
		Position = position; Normal = normal; TexCoords.x = tx; TexCoords.y = ty;
	}
	Vertex() {}
};

struct Material {
	glm::vec3 diffuseColour, specularColour;
	float shininess, reflectivity;
};

class Mesh {
private:
	unsigned int VBO, EBO;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;
	Material material;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, bool setup = true);
	Mesh();
	// render the mesh
	void Draw(Shader* shader, bool setMaterialData = true);
	void SetMaterialData(Shader* shader);
	//update data
	void AddMeshData(Mesh* mesh, glm::vec3 offset, glm::vec3 scale, float spin);
	void SetupMesh();
};