#include "Model.h"

Model::Model(std::string path){
	LoadModel(path);
}

bool Model::Draw(Shader* shader, bool setMaterialData) {
	if (loaded) {
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i]->Draw(shader, setMaterialData);
		return true;
	}
	return false;
}

void Model::LoadModel(std::string path) {
	unsigned int meshCount = 0;

	//load meta data
	std::vector<Material> materials;
	std::string dataPath = "res/meshes/" + path + "/data.txt";
	std::ifstream inFile;
	inFile.open(dataPath.c_str());
	if (inFile.is_open()) {
		inFile >> meshCount;
		for (int i = 0; i < meshCount; i++) {
			materials.push_back(Material());

			inFile >> materials.at(i).diffuseColour.r;
			inFile >> materials.at(i).diffuseColour.g;
			inFile >> materials.at(i).diffuseColour.b;

			inFile >> materials.at(i).specularColour.r;
			inFile >> materials.at(i).specularColour.g;
			inFile >> materials.at(i).specularColour.b;

			inFile >> materials.at(i).shininess;
			inFile >> materials.at(i).reflectivity;
		}
		inFile.close();
		for (int i = 0; i < meshCount; i++) {
			Mesh* mesh = LoadSingleMesh("res/meshes/" + path + "/" + std::to_string(i) + ".obj", true);
			mesh->material = materials.at(i);
			meshes.push_back(mesh);
		}
		loaded = true;
	}
	else {
		std::cout << "failed to load '" + path + "' data." << std::endl;
	}
}

Mesh* Model::LoadSingleMesh(std::string path, bool setupMesh) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);//

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int lastMeshIndex = 0;
	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* aimesh = scene->mMeshes[i];
		
		for (unsigned int i = 0; i < aimesh->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector;
			// positions
			vector.x = aimesh->mVertices[i].x;// * 0.1f
			vector.y = aimesh->mVertices[i].y;
			vector.z = aimesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = aimesh->mNormals[i].x;
			vector.y = aimesh->mNormals[i].y;
			vector.z = aimesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (aimesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = aimesh->mTextureCoords[0][i].x;
				vec.y = aimesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
			aiFace face = aimesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j] + lastMeshIndex);
		}

		lastMeshIndex = vertices.size();
	}
	

	return new Mesh(vertices, indices, setupMesh);
}
