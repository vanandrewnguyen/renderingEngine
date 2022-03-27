#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

/*
EBO's are element buffer objects, which tell us what vertices are drawn in which order. In this way we don't overlap
triangles, and can build up complex shapes from triangles.
*/

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