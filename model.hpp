#ifndef MODEL_H
#define MODEL_H

#include <json/json.h>
#include "mesh.hpp"
#include "materials.hpp"

using json = nlohmann::json;

class Model {
public:
	// Constructor
	Model(const char* file);

	// Functions
	void draw(Shader& shader, Camera& camera, Material &mat, std::vector<float> worldTranslation);

private:
	std::vector<unsigned char> getData();

	// Interprets the binary data into floats, indices, and textures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	void loadMesh(unsigned int meshIndex);
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

	// Functions to group float234's 
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

	
private:
	const char* file;
	json JSON;
	std::vector<unsigned char> data;

	// Storing meshes and given transformations (because we may have multiple meshes which need to each be transformed)
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	std::vector<std::string> loadedTexName; // stores names of textures so we don't read them twice
	std::vector<Texture> loadedTex;
};

#endif