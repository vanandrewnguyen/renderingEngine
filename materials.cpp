#include "materials.hpp"

Material::Material(float reflVal, float IORVal, float transVal, std::vector<float> albedo, Shader& shaderProgram) {
	// These are passed into the vertex shader which is then passed into the fragment
	reflectivity = reflVal;
	IOR = IORVal;
	translucencyVal = transVal;
	matAlbedo = albedo;

	// Pass directly to the fragment shader
	refreshMaterialProperties(shaderProgram);
}

void Material::refreshMaterialProperties(Shader& shaderProgram) {
	// Pass directly to the fragment shader
	// Needs to be called everytime a material is updated or the model switches materials
	shaderProgram.Activate();
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "matAlbedo"), matAlbedo.at(0), matAlbedo.at(1), matAlbedo.at(2));
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "matReflectivity"), reflectivity);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "matIOR"), IOR);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "matTranslucency"), translucencyVal);
}