#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO {
public:
	EBO(std::vector <GLuint>& indices);

	// Functions
	void Bind();
	void Unbind();
	void Delete();
public:
	GLuint ID;
};

#endif