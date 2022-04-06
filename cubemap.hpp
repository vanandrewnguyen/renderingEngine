#ifndef CUBEMAP_CLASS_H
#define CUBEMAP_CLASS_H

#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shaders/shaderClass.hpp"
#include "camera.hpp"

class Cubemap {
public:
	Cubemap();

	void draw(float windowWidth, float windowHeight, Shader& skyboxShader, Camera& camera);
public:
	GLuint skyboxVAOID;
	GLuint skyboxVBOID;
	GLuint skyboxEBOID;
	GLuint skyboxTexID;
};

#endif