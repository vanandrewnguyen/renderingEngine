#include "materials.hpp"

Material::Material(float reflVal, float IORVal, int isTranslucentVal) {
	reflectivity = reflVal;
	IOR = IORVal;
	isTranslucent = isTranslucentVal;
}