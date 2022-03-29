#ifndef MESH_H
#define MESH_H

#include <string>

#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"
#include "texture.hpp"

class Mesh {
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// Mesh constructor
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh with default values
	void Draw(Shader& shader, Camera& camera);
public:
	VAO VAO;
};

#endif