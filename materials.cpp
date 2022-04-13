#include "materials.hpp"

Material::Material(float reflVal, float IORVal, int isTranslucentVal, std::vector<float> albedo, Shader& shaderProgram) {
	// These are passed into the vertex shader which is then passed into the fragment
	reflectivity = reflVal;
	IOR = IORVal;
	isTranslucent = isTranslucentVal;
	matAlbedo = albedo;

	// Pass directly to the fragment shader
	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "matAlbedo"), matAlbedo.at(0), matAlbedo.at(1), matAlbedo.at(2));
}

void Material::refreshMaterialProperties(Shader& shaderProgram) {
	// Pass directly to the fragment shader
	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "matAlbedo"), matAlbedo.at(0), matAlbedo.at(1), matAlbedo.at(2));
}