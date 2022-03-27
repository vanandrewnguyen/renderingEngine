#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform float iTime;
uniform sampler2D tex0;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

float getSpec(float specStrength, vec3 viewDir, vec3 reflDir, int specPow) {
    float specAmount = pow(max(dot(viewDir, reflDir), 0.0), specPow);
    float spec = specAmount * specStrength;
    return spec;
}

void main() {
    vec3 col = texture(tex0, texCoord).rgb;

    // Lighting
    float ambientLight = 0.1;
    vec3 currNormal = normalize(normal);
    vec3 lightDir = normalize(lightPos - currentPos);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(0.5, normalize(camPos - currentPos), reflect(-lightDir, normal), 8);
    vec3 lightOut = (diff + ambientLight + spec) * lightColour.rgb;

    col *= lightOut;
    FragColor = vec4(col, 1.0);
} 