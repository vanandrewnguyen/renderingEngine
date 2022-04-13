#ifndef MATERIALS_CLASS_H
#define MATERIALS_CLASS_H

#include <glad/glad.h>
#include <vector>
#include "Shaders/shaderClass.hpp"

class Material {
public:
	Material(float reflVal, float IORVal, int isTranslucentVal, std::vector<float> albedo, Shader& shaderProgram);
	void refreshMaterialProperties(Shader& shaderProgram);

public:
	// Note with materials, we're choosing to pass it through the vertex shader through to the fragment shader in case we 
	// would like to manipulate vertices in the future depending on materials.
	float reflectivity;
	float IOR;
	int isTranslucent;
	std::vector<float> matAlbedo;
};

#endif