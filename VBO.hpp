#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

/*
VBOs store large number of vertices in the GPU memory. Once data is in the graphics card the vertex shader has almost 
instant access to the vertex data.
*/

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 texUV;
};

class VBO {
public:
	VBO(std::vector <Vertex>& vertices);

	// Functions for binding, unbinding, freeing, etc.
	void Bind();
	void Unbind();
	void Delete();

public:
	GLuint ID;
};

#endif