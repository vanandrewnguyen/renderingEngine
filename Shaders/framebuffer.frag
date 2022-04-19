#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform float screenWidth;
uniform float screenHeight;
uniform sampler2D screenTexture;
uniform vec3 cameraPos;
uniform vec3 cameraOrientation;

#define MAXSTEPS 100
#define MAXDIS 100.0
#define SURFDIS 0.01

float offsetX = 1.0 / screenWidth;
float offsetY = 1.0 / screenHeight;

vec2 offset[9] = vec2[] (
	vec2(-offsetX, offsetY),  vec2( 0.0f, offsetY),  vec2(offsetX, offsetY),
    vec2(-offsetX, 0.0f),     vec2( 0.0f, 0.0f),     vec2( offsetX, 0.0f),
    vec2(-offsetX, -offsetY), vec2( 0.0f, -offsetY), vec2( offsetX, -offsetY) 
);

// With kernals, you'd want them to add up to 1. If it's more than 1 the final output will be brighter, vice versa with <1
float kernal[9] = float[](
	/* Edge detection
	1,  1, 1,
    1, -8, 1,
    1,  1, 1
	*/
	/* Sharpness
	0, -1, 0,
	-1, 5, -1,
	0, -1, 0
	*/
	/* Strange VHS Grain
	-2, -1, 0,
	-1, 1, 1,
	0, 1, 2
	*/
	/* Blur
	1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
	*/
	0, 0, 0,
	0, 1, 0,
	0, 0, 0
);

void main() {
	vec3 col = vec3(0);
	// Add the kernel
	for (int i = 0; i < 9; i++) {
		col += texture(screenTexture, texCoords + offset[i]).rgb * kernal[i]; 
	}

	FragColor = vec4(col, 1.0);
}

/*
float sdfSphere(vec3 pos, vec3 center, float rad) {
    pos -= center;
    return length(pos) - rad;
}

float scene(vec3 pos) {
	float sphereDis = sdfSphere(pos, vec3(0,1,3), 0.5);

	return sphereDis;
}

float rayMarch(vec3 rayOrigin, vec3 rayDir) {
	float disFromOrigin = 0.0;

	for (int i = 0; i < MAXSTEPS; i++) {
		vec3 pos = rayOrigin + rayDir * disFromOrigin;
		float disToScene = scene(pos);
		disFromOrigin += disToScene;

		if (disFromOrigin > MAXDIS || disToScene < SURFDIS) break;
	}

	return disFromOrigin;
}

vec3 getNormal(vec3 pos) {
	float dis = scene(pos);
	vec2 eps = vec2(0.01, 0);
	vec3 normal = dis - vec3(scene(pos - eps.xyy), scene(pos - eps.yxy), scene(pos - eps.yyx));
	return normalize(normal);
}

float getLight(vec3 pos) {
	vec3 lightPos = vec3(0, 5, 1);
	vec3 lightRay = normalize(lightPos - pos);
	vec3 normal = getNormal(pos);

	float diff = dot(normal, lightRay);
	return diff;
}

void main() {
	vec3 col = vec3(0);
	vec2 uv = (texCoords.xy - vec2(0.5, 0.5)) * 2.0;
	
	vec3 rayOrigin = vec3(0, 1, 0);
	vec3 rayDir = normalize(vec3(uv.x, uv.y, 1.0));

	float dis = rayMarch(rayOrigin, rayDir);
	vec3 pos = rayOrigin + rayDir * dis;
	float ambient = 0.2;
	float diff = 0.0;
	if (dis < MAXDIS) {
		diff = max(max(getLight(pos), 0), ambient);
		col = vec3(diff);
	} else {
		col = texture(screenTexture, texCoords).rgb;
	}

	FragColor = vec4(col, 1.0);
}
*/
