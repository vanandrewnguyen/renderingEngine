#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.hpp"

/*
A vertex array object is needed so OpenGL knows what to do with vertex inputs, otherwise it won't draw anything.
VAO's are like structs / wrappers of draw calls, pointers to VBO's.
*/

class VAO {
public:
	// Constructor that generates a VAO ID
	VAO();

	// Functions for binding, unbinding, freeing, etc.
	void LinkVBO(VBO& VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();

public:
	// ID ref for Vertex Array Object
	GLuint ID;
};
#endif