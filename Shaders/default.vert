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
    // Pass variable to frag
    // rot is neg, play with it. 
    currentPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0));
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    surfNormal = aNormal;
    vertColour = aColour;

    // Output vert
    gl_Position = camMatrix * vec4(currentPos, 1.0); 
}