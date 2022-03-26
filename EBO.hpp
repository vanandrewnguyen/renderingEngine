#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

class EBO {
public:
	EBO(GLuint* indices, GLsizeiptr size);

	// Functions
	void Bind();
	void Unbind();
	void Delete();
public:
	GLuint ID;
};

#endif