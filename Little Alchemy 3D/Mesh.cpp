#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, bool setup)
{
	this->vertices = vertices;
	this->indices = indices;

	if (setup) SetupMesh();
}

Mesh::Mesh() {

}

void Mesh::Draw(Shader* shader, bool setMaterialData) {
	if (setMaterialData) {
		SetMaterialData(shader);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetMaterialData(Shader* shader) {
	shader->setVec3("material.diffuse_colour", material.diffuseColour);
	shader->setVec3("material.specular_colour", material.specularColour);
	shader->setFloat("material.shininess", material.shininess);
	shader->setFloat("material.reflectivity", material.reflectivity);
}

void Mesh::AddMeshData(Mesh* mesh, glm::vec3 offset, glm::vec3 scale, float spin) {
	glm::mat4 spinMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(spin), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int indexBegin = this->vertices.size();
	for (int i = 0; i < mesh->vertices.size(); i++) {
		glm::vec3 position = mesh->vertices[i].Position;
		position.x *= scale.x;
		position.y *= scale.y;
		position.z *= scale.z;

		position = spinMatrix * glm::vec4(position,1.0f);

		position.x += offset.x;
		position.y += offset.y;
		position.z += offset.z; 

		this->vertices.push_back(Vertex(position, mesh->vertices[i].Normal, mesh->vertices[i].TexCoords.x, mesh->vertices[i].TexCoords.y));
	}

	for (int i = 0; i < mesh->indices.size(); i++) {
		this->indices.push_back(indexBegin + mesh->indices[i]);
	}
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}