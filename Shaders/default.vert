#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 surfNormal;
out vec3 currentPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    currentPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = camMatrix * vec4(currentPos, 1.0); 
    texCoord = aTex;
    surfNormal = aNormal;
}