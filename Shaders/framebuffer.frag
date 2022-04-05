#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform float screenWidth;
uniform float screenHeight;
uniform sampler2D screenTexture;

float offsetX = 1.0 / screenWidth;
float offsetY = 1.0 / screenHeight;

vec2 offset[9] = vec2[] (
	vec2(-offsetX, offsetY),  vec2( 0.0f, offsetY),  vec2(offsetX, offsetY),
    vec2(-offsetX, 0.0f),     vec2( 0.0f, 0.0f),     vec2( offsetX, 0.0f),
    vec2(-offsetX, -offsetY), vec2( 0.0f, -offsetY), vec2( offsetX, -offsetY) 
);

// With kernals, you'd want them to add up to 1. If it's more than 1 the final output will be brighter, vice versa with <1
float kernal[9] = float[](
	1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void main() {
	vec3 col = vec3(0);

	// Add the kernel
	for (int i = 0; i < 9; i++) {
		col += texture(screenTexture, texCoords + offset[i]).rgb * kernal[i]; 
	}

	FragColor = vec4(col, 1.0);
}

