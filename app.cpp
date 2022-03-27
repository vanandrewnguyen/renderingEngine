#include "app.hpp"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 600

// Vertices coordinates
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

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
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
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(vertices), (void*)0);
    VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(vertices), (void*)(3 * sizeof(float)));
    
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    // Textures
    int widthImg, heightImg, numColourChannel;
    unsigned char* bytes = stbi_load("texBrickWall.png", &widthImg, &heightImg, &numColourChannel, 0);

    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    // Tex settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // Mip maps
    glGenerateMipmap(GL_TEXTURE_2D);
    // Delete data
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Get uniforms
    int timeUni = glGetUniformLocation(defaultShader.ID, "iTime");
    GLuint tex0Uni = glGetUniformLocation(defaultShader.ID, "tex0");
    defaultShader.Activate();
    glUniform1i(tex0Uni, 0);

    // Render loop
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands //
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        defaultShader.Activate();

        // Pass uniforms
        float timeVal = glfwGetTime();
        glUniform1f(timeUni, timeVal);
        glBindTexture(GL_TEXTURE_2D, texID);

        VAO.Bind();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    glDeleteTextures(1, &texID);
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
