#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "Shaders/shaderClass.hpp"

class Camera {
public:
	// Camera constructor 
	Camera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader
	void updateMatrix(float FOVDeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);

	// Handles camera inputs i.e. movement and panning
	void Inputs(GLFWwindow* window);
public:
	// Store main vectors of the camera, namely it's lookat position (camera front) and up vector
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Basic lock to prevent camera jumping around
	bool firstClick = true;

	// Window width and height
	int width;
	int height;

	// Speed and sensitivity of camera when panning
	float speed = 0.05f;
	float sensitivity = 100.0f;
	float camMoveSpd = 0.0025f;
	float camMoveSpdFast = 0.005f;
};
#endif