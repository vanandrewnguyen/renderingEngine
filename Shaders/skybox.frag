#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skyboxTex;

void main() {
	FragColor = texture(skyboxTex, texCoords);
}