#include "app.hpp"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800


GLfloat vertices[] = {
    // Vert Coord       // Tex Coord
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
};
GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

// Constructor (init variables)
App::App() {

}

// Main app loop
int App::loop() {

    // Init GLFW Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* currWindow = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Rendering Engine", NULL, NULL);
    if (currWindow == NULL) {
        std::cout << "Error: Failed to create a GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(currWindow);

    // Init GLAD (manager of functions for OpenGL, need to init before using openGL functions)
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Error: Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Set viewport as lower left corner (shading coords)
    registerNewFramebufferSize(currWindow, WINDOWWIDTH, WINDOWHEIGHT);

    // Setup shaders
    Shader defaultShader("Shaders/default.vert", "Shaders/default.frag");
    
    // Generates VAO and bind
    VAO VAO;
    VAO.Bind();

    // Generates VBO and EBO and link to vert array, the link VAO to VBO
    VBO VBO(vertices, sizeof(vertices));
    EBO EBO(indices, sizeof(indices));
    // VBO / slot num / vec? / type / sizeof(float) * how long a line is (e.g. 3, 8 args), cast
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);

    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    // Textures
    Texture brickTex("Textures/texBrickWall.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    brickTex.assignTexUnit(defaultShader, "tex0", 0);

    // Uniforms
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");

    // Render loop
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        defaultShader.Activate();

        // Uniforms
        glUniform1f(uniTime, glfwGetTime());
        brickTex.Bind();
        VAO.Bind();
        
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    brickTex.Delete();
    defaultShader.Delete();
    glfwTerminate();
    return 0;
}

void App::registerNewFramebufferSize(GLFWwindow* currWindow, int width, int height) {
    glViewport(0, 0, width, height);
}

void App::handleUserInput(GLFWwindow* currWindow) {
    if (glfwGetKey(currWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(currWindow, true);
    }
}
