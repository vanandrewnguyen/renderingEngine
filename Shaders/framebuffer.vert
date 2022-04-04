#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main() {
	// Represent gl_pos as a 2d map with no z depth
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoords = aTexCoords;
}