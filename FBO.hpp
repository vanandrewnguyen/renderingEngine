#ifndef FBO_CLASS_H
#define FBO_CLASS_H

#include <iostream>
#include<stdlib.h>
#include<glad/glad.h>

class FBO {
public:
	// Constructor
	FBO(bool toggleMSAA);

	// Functions
	void Bind();
	void Unbind();
	void BindTex();
	void Delete();
public:
	GLuint FBOID;
	GLuint texID;
	GLuint RBOID;
	GLuint rectVAOID;
	GLuint rectVBOID;

	int windowHeight;
	int windowWidth;
};

#endif