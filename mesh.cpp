#include "mesh.hpp"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures) {
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

    // Generates VAO (in hpp) and bind
    VAO.Bind();

    // Generates VBO and EBO and link to vert array
    VBO VBO(vertices);
    EBO EBO(indices);
    // VBO / slot num / vec? / type / sizeof(float) * how long a line is (e.g. 3, 8 args), cast
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // 3 -> vec3 in prev
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // 3+3 -> vec2 in prev
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    // Unbind all 
    VAO.Unbind();
    VBO.Unbind();
	EBO.Unbind();
}

// Draws the mesh with default values
void Mesh::draw(Shader& shader, Camera& camera, Material mat, glm::mat4 matrix,
				glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {
	// Bind shader to access uniforms
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	for (unsigned int i = 0; i < textures.size(); i++) {
		// Increment the number of textures of each type
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		} else if (type == "specular") {
			num = std::to_string(numSpecular++);
		}

		// Convert this into e.g. diffuse0, specular0, diffuse1, diffuse2, etc
		textures[i].assignTexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	// Pass to camMatrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	// New stuff from mesh matrices + transformations
	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Push material to vertex shader (to be passed onto fragment shader)
	glUniform1f(glGetUniformLocation(shader.ID, "materialReflectivity"), mat.reflectivity);
	glUniform1f(glGetUniformLocation(shader.ID, "materialIOR"), mat.IOR);
	glUniform1i(glGetUniformLocation(shader.ID, "materialIsTranslucent"), mat.isTranslucent);

	// Draw mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}