#include"VAO.hpp"

// Constructor that generates a VAO ID
VAO::VAO(VBO VBO, float* vertices, int verticesNum) {
	glGenVertexArrays(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, verticesNum, GL_FLOAT, GL_FALSE, verticesNum * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

// Binds the VAO
void VAO::Bind() {
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::Unbind() {
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}