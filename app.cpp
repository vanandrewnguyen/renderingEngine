#include "app.hpp"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800


GLfloat vertices[] = {
    // Vert Coord        // Tex Coord // Normals
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};
GLuint indices[] = {  
    0, 1, 2,
    0, 2, 3
};

GLfloat lightVertices[] = {
    // Vert coord
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
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
    Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

    // Generates VAO and bind
    VAO VAO1;
    VAO1.Bind();

    // Generates VBO and EBO and link to vert array, the link VAO to VBO
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));
    // VBO / slot num / vec? / type / sizeof(float) * how long a line is (e.g. 3, 8 args), cast
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);

    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Repeat for Light VAO, VBO, EBO
    VAO lightVAO;
    lightVAO.Bind();
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    // Model Translations
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 subjectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 subjectModel = glm::mat4(1.0f);
    subjectModel = glm::translate(subjectModel, subjectPos);

    // Textures
    Texture woodTex("Textures/texWoodFloor.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    woodTex.assignTexUnit(defaultShader, "tex0", 0);
    Texture woodDispTex("Textures/texWoodFloorDisp.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    woodDispTex.assignTexUnit(defaultShader, "tex1", 1);

    // Uniforms
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");
    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    defaultShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(subjectModel));
    glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColour"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Camera
    Camera camera(WINDOWWIDTH, WINDOWHEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Render loop
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        defaultShader.Activate();
        
        // Camera
        camera.Inputs(currWindow);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);
        camera.Matrix(defaultShader, "camMatrix");
        GLuint camPosLoc = glGetUniformLocation(defaultShader.ID, "camPos");
        glUniform3f(camPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

        // Uniforms
        glUniform1f(uniTime, glfwGetTime());
        woodTex.Bind();
        woodDispTex.Bind();
        VAO1.Bind();
        
        // Draw our wall model
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        // Lighting
        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    woodTex.Delete();
    woodDispTex.Delete();
    defaultShader.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();
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
