#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<iostream>

/*
VBOs store large number of vertices in the GPU memory. Once data is in the graphics card the vertex shader has almost 
instant access to the vertex data.
*/

class VBO {
public:
	VBO(float* vertices, int verticesNum);

	// Functions for binding, unbinding, freeing, etc.
	void Bind();
	void Unbind();
	void Delete();

public:
	GLuint ID;
};

#endif