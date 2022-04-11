#version 330 core

// First layout is for input, second layout is for output

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// These are passed to frag
out vec3 geomCurrentPos;
out vec3 geomSurfNormal;
out vec3 geomVertColour;
out vec2 geomTexCoord;
out float geomMatReflectivity;
out float geomMatIOR;
out float geomMatIsTranslucent;

// Take data from vert
in DATA {
	vec3 currentPos;
	vec3 surfNormal;
	vec3 vertColour;
	vec2 texCoord;
	mat4 projection;
	float matReflectivity;
	float matIOR;
	float matIsTranslucent;
} data_in[];

void main() {
	gl_Position = data_in[0].projection * gl_in[0].gl_Position;

    geomSurfNormal = data_in[0].surfNormal;
    geomVertColour = data_in[0].vertColour;
    geomTexCoord = data_in[0].texCoord;

    EmitVertex();

    gl_Position = data_in[1].projection * gl_in[1].gl_Position;

    geomSurfNormal = data_in[1].surfNormal;
    geomVertColour = data_in[1].vertColour;
    geomTexCoord = data_in[1].texCoord;

    EmitVertex();

    gl_Position = data_in[2].projection * gl_in[2].gl_Position;

    geomSurfNormal = data_in[2].surfNormal;
    geomVertColour = data_in[2].vertColour;
    geomTexCoord = data_in[2].texCoord;

    EmitVertex();

    EndPrimitive();
}