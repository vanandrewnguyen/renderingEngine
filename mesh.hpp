#ifndef MESH_H
#define MESH_H

#include <string>

#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "materials.hpp"

class Mesh {
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// Mesh constructor
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh with default values
	void draw
	(
		Shader& shader,
		Camera& camera,
		Material mat,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
public:
	VAO VAO;
};

#endif