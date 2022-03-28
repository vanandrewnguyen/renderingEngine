#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 surfNormal;
in vec3 currentPos;

uniform float iTime;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

// Grab specular light based on normal and viewing angle
float getSpec(float specStrength, vec3 lightDir, vec3 normal, int specPow) {
    vec3 viewDir = normalize(camPos - currentPos);
	vec3 reflDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflDir), 0.0), specPow);
    float spec = specAmount * specStrength;
    return spec;
}

vec3 getPointLight(float ambient, vec3 normal, vec3 lightDir) {
    // This light dims after a certain extent, which is modulated using intensity with the inverse square law.
	// Lights objects in all directions equally.

    vec3 col;

    // Light attenuation
    vec3 lightRay = lightPos - currentPos;
    float rayDis = length(lightRay);
    float a = 2.0; // for a approaching 1 and b approaching 0 we have a weaker decay
    float b = 0.5; 
    float intensity = 1.0 / (a * rayDis * rayDis + b * rayDis + 1.0);
    lightDir = normalize(lightRay);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(0.5, lightDir, normal, 16);
    col = (texture(tex0, texCoord) * (diff * intensity + ambient) + 
           texture(tex1, texCoord).r * spec * intensity).rgb * lightColour.rgb;

    return col;
}

vec3 getDirectionalLight(float ambient, vec3 normal, vec3 lightDir) {
	// This light casts a big light, which simulates rays which are mostly parallel to each other.
	// Can be used for sunlight / natural light.

	// Unlike the pointLight this does not include intensity, since it's implied to not decay as quickly
    vec3 col;

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(0.5, lightDir, normal, 16);
	col = (texture(tex0, texCoord) * (diff + ambient) + 
           texture(tex1, texCoord).r * spec).rgb * lightColour.rgb;
    
    return col;
}

vec4 spotLight(float ambient, vec3 normal, vec3 lightDir, vec3 spotDir) {
	// This light casts a circle of light.
    vec3 col;

	// Width of inner and outer cone
	float outerCone = 0.80f;
	float innerCone = 0.95f;

	// Angle and intensity of light (spotDir is vec3(0.0f, -1.0f, 0.0f) for straight down)
	float angle = dot(spotDir, -lightDir);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(0.5, lightDir, normal, 16);
	col = (texture(tex0, texCoord) * (diff * intensity + ambient) + 
           texture(tex1, texCoord).r * spec * intensity).rgb * lightColour.rgb;

    return col;
}

void main() {
    // Lighting
    float ambient = 0.1;
    vec3 normal = normalize(surfNormal);
    vec3 lightDir = normalize(lightPos - currentPos);

    // Lighting
    vec3 col = getPointLight(ambient, normal, lightDir);
    FragColor = vec4(col, 1.0);
} 