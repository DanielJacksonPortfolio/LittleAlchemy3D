
#pragma once

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Constants.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
private:
	void LoadModel(std::string path);

public:
	bool loaded = false;
	std::vector<Mesh*> meshes;

	Model(std::string path);
	bool Draw(Shader* shader, bool setMaterialData = true);
	static Mesh* LoadSingleMesh(std::string path, bool setupMesh = false);
};