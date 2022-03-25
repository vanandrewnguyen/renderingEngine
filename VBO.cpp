#include "VBO.hpp"
#include <iterator>

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(float* vertices, int verticesNum) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, verticesNum, GL_FLOAT, GL_FALSE, verticesNum * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

// Binds the VBO
void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}