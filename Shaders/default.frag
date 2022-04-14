#version 330 core
out vec4 FragColor;

// Has to be in the same order as in vert. shd
in vec3 currentPos;
in vec3 surfNormal;
in vec3 vertColour;
in vec2 texCoord;

uniform float iTime;
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform samplerCube skyboxTex;
uniform float matReflectivity;
uniform float matIOR;
uniform float matTranslucency;
uniform vec3 matAlbedo;

// LIGHTING ////////////////////////////////////////////////////////////

// Grab specular light based on normal and viewing angle
float getSpec(float diffuse, float specStrength, vec3 lightDir, vec3 normal, int specPow) {
    float spec = 0.0;

    if (diffuse != 0.0) {
        vec3 viewDir = normalize(camPos - currentPos);
	    vec3 reflDir = reflect(-lightDir, normal);
    
        // Blin Phong
        vec3 midVec = normalize(viewDir + lightDir);
        float specAmount = pow(max(dot(viewDir, midVec), 0.0), specPow);
        spec = specAmount * specStrength;
    }

    return spec;
}

vec3 getPointLight(float ambient, vec3 normal, vec3 currLightPos) {
    // This light dims after a certain extent, which is modulated using intensity with the inverse square law.
	// Lights objects in all directions equally.

    vec3 col;

    // Light attenuation
    vec3 lightRay = currLightPos - currentPos;
    vec3 lightDir = normalize(lightRay);
    float rayDis = length(lightRay);
    float a = 1.0; // for a approaching 1 and b approaching 0 we have a weaker decay
    float b = 0.25; 
    float intensity = 1.0 / (a * rayDis * rayDis + b * rayDis + 1.0);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(diff, 0.5, lightDir, normal, 16);
    col = (texture(diffuse0, texCoord) * (diff * intensity + ambient) + 
           texture(specular0, texCoord).r * spec * intensity).rgb * lightColour.rgb;

    return col;
}

vec3 getDirectionalLight(float ambient, vec3 normal, vec3 currLightPos) {
	// This light casts a big light, which simulates rays which are mostly parallel to each other.
	// Can be used for sunlight / natural light.

	// Unlike the pointLight this does not include intensity, since it's implied to not decay as quickly
    vec3 col;
    vec3 lightRay = currLightPos - currentPos;
    vec3 lightDir = normalize(lightRay);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(diff, 0.5, lightDir, normal, 16);
	col = (texture(diffuse0, texCoord) * (diff + ambient) + 
           texture(specular0, texCoord).r * spec).rgb * lightColour.rgb;
    
    return col;
}

vec4 getSpotLight(float ambient, vec3 normal, vec3 spotDir, vec3 currLightPos) {
	// This light casts a circle of light.
    vec3 col;
    vec3 lightRay = currLightPos - currentPos;
    vec3 lightDir = normalize(lightRay);

	// Width of inner and outer cone
	float outerCone = 0.80f;
	float innerCone = 0.95f;

	// Angle and intensity of light (spotDir is vec3(0.0f, -1.0f, 0.0f) for straight down)
	float angle = dot(spotDir, -lightDir);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    // Phong
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = getSpec(diff, 0.5, lightDir, normal, 16);
	col = (texture(diffuse0, texCoord) * (diff * intensity + ambient) + 
           texture(specular0, texCoord).r * spec * intensity).rgb * lightColour.rgb;

    return col;
}

// MATH ////////////////////////////////////////////////////////////

float computeLinearDepth(float depth, float near, float far) {
    float linearDepth = (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
    return linearDepth;
}

float computeFogDepth(float depth, float steepness, float offset, float near, float far) {
    float zDepth = computeLinearDepth(depth, near, far);
    float fogDepth = (1 / (1 + exp(-steepness * (zDepth - offset)))); 
    return fogDepth;
}

// MAIN ////////////////////////////////////////////////////////////

void main() {
    // Moving the light
    vec3 currLightPos = lightPos;
    currLightPos.x += 0.5;
    currLightPos.yz += vec2(sin(iTime), cos(iTime)) * 1.0;

    // Lighting
    float ambient = 0.1;
    vec3 normal = normalize(surfNormal);
    vec3 lightDir = normalize(currLightPos - currentPos);

    vec3 col;

    // Depth Buffer (simulating fog)
    float near = 0.2;
    float far = 32.0;
    float depth = computeFogDepth(gl_FragCoord.z, 0.5, 1.0, near, far);
    vec3 fogCol = vec3(0.0, 0.05, 0.1);
    vec3 lightPassCol = matAlbedo * getPointLight(ambient, normal, currLightPos) * vertColour * (1.0 - depth) + (depth * fogCol);

    // Materials
    vec3 viewDir = normalize(currentPos - camPos);
    vec3 sampleDir = reflect(viewDir, normal);
    // Reflectivity
    if (matReflectivity != 0.0) {
        col = (1.0 - matReflectivity) * lightPassCol + matReflectivity * texture(skyboxTex, sampleDir).rgb;
    } else {
        col = lightPassCol;
    }
    // Translucency
    if (matTranslucency != 0.0) {
        float ratio = 1.0 / matIOR;
        // Add chromatic abberation through distorting the IOR for rgb channels on cube map sample
        float increment = 0.05;
        float abb = -increment;
        for (int i = -1; i <= 1; i++) {
            sampleDir = refract(viewDir, normal, 1.0 / (matIOR + abb));
            // Avoid total internal reflection
            if (dot(sampleDir, sampleDir) == 0.0) {
                sampleDir = reflect(viewDir, normal);
            }
            if (abb < 0) {
                col.r = mix(col.r, texture(skyboxTex, sampleDir).r, matTranslucency); 
            } else if (abb == 0) {
                col.g = mix(col.g, texture(skyboxTex, sampleDir).g, matTranslucency); 
            } else if (abb > 0) {
                col.b = mix(col.b, texture(skyboxTex, sampleDir).b, matTranslucency);  
            }
            abb += increment;
        }
    }

    // Out
    FragColor = vec4(col, 1.0);
} 

//Example of 2d textures on UV
/*
vec2 UV = gl_FragCoord.xy * vec2(0.5);
float checker = mod(floor(UV.x), 2.0) + mod(floor(UV.y), 2.0);
bool isEven = mod(checker, 2.0) == 0.0;
col *= (isEven) ? vec3(1.0, 0.0, 0.0) : vec3(1.0);
*/