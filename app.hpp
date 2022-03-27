#ifndef APP_H
#define APP_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders/shaderClass.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "texture.hpp"
#include "camera.hpp"

class App {
public:
	// Constructor
	App();

	// Functions
	int loop();
	void registerNewFramebufferSize(GLFWwindow* currWindow, int width, int height);
	void handleUserInput(GLFWwindow* currWindow);
};

#endif 