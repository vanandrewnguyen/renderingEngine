#ifndef MATERIALS_CLASS_H
#define MATERIALS_CLASS_H

#include <glad/glad.h>
#include <vector>
#include "Shaders/shaderClass.hpp"

class Material {
public:
	Material(float reflVal, float IORVal, float transVal, std::vector<float> albedo, Shader& shaderProgram);
	void refreshMaterialProperties(Shader& shaderProgram);

public:
	float reflectivity;
	float IOR;
	float translucencyVal;
	std::vector<float> matAlbedo;
};

#endif