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

float getSpec(float specStrength, vec3 lightDir, vec3 normal, int specPow) {
    vec3 viewDir = normalize(camPos - currentPos);
	vec3 reflDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflDir), 0.0), specPow);
    float spec = specAmount * specStrength;
    return spec;
}

void main() {
    // Lighting
    float ambient = 0.2;
    vec3 normal = normalize(surfNormal);
    vec3 lightDir = normalize(lightPos - currentPos);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(0.5, lightDir, normal, 16);

    vec3 col = (texture(tex0, texCoord) * (diff + ambient) + texture(tex1, texCoord).r * spec).rgb * lightColour.rgb;
    //col += max(currentPos.xyz, 0.0);
    FragColor = vec4(col, 1.0);
} 