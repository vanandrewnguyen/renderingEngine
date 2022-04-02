#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColour;
layout (location = 3) in vec2 aTex;

out vec3 currentPos;
out vec3 surfNormal;
out vec3 vertColour;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
	currentPos = aPos;
	surfNormal = aNormal;
	vertColour = aColour;
	texCoord = aTex;
	gl_Position = vec4(currentPos, 1.0);
}