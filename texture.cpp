#include "texture.hpp"

Texture::Texture(const char* image, const char* texType, GLuint slot) {
	// Assign self type
	type = texType;

	// Store image into bytes
	int widthImg, heightImg, numColourChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColourChannel, 0);

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Texture config render params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Check what type of color channels the texture has and load it accordingly
	if (numColourChannel == 4) {
		// png
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,	widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	} else if (numColourChannel == 3) {
		// jpg
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,	widthImg, heightImg, 0, GL_RGB,	GL_UNSIGNED_BYTE, bytes);
	} else if (numColourChannel == 1) {
		// mono
		glTexImage2D(GL_TEXTURE_2D,	0, GL_RGBA,	widthImg, heightImg, 0,	GL_RED,	GL_UNSIGNED_BYTE, bytes);
	}

	// Assigns image to tex obj
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Cleanup and unbind
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::assignTexUnit(Shader& shaderProgram, const char* uniform, GLuint unit) {
	// Uniform location and activate program
	GLuint texUni = glGetUniformLocation(shaderProgram.ID, uniform);
	shaderProgram.Activate();
	// Pass uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	// Binding multipler textures to the same shader
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}