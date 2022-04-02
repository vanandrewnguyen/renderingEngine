#version 330 core
out vec4 FragColor;

in vec3 currentPos;
in vec3 surfNormal;
in vec3 vertColour;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

void main() {
	FragColor = vec4(vertColour, 1.0f);
}