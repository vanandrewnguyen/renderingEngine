#ifndef MATERIALS_CLASS_H
#define MATERIALS_CLASS_H

#include <glad/glad.h>

class Material {
public:
	Material(float reflVal, float IORVal, int isTranslucentVal);

public:
	// Note with materials, we're choosing to pass it through the vertex shader through to the fragment shader in case we 
	// would like to manipulate vertices in the future depending on materials.
	float reflectivity;
	float IOR;
	int isTranslucent;
};

#endif