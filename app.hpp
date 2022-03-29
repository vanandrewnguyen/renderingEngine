#ifndef APP_H
#define APP_H

#include "mesh.hpp"

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