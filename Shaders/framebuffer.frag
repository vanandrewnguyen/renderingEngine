#version 330 core

out vec4 FragColour;

in vec2 texCoords;

uniform sampler2D screenTex;

void main() {
	FragColour = texture(screenTex, texCoords);
}

