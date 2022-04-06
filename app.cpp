#include "app.hpp"

/*
Vertex vertices[] = {
    // Vert Coord // Normals // Colour // Tex Coord
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};
GLuint indices[] = {  
    0, 1, 2,
    0, 2, 3
};

Vertex lightVertices[] = {
    // Vert coord
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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
*/

// Frame buffer vert + indicies (rect with no transforms)
float frameVert[] = {
    // Coords    // texCoords
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,

    1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f
}; 

// Constructor (init variables)
App::App() {
    windowWidth = 800;
    windowHeight = 800;
}

// Main app loop
int App::loop() {

    // Init GLFW Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* currWindow = glfwCreateWindow(windowWidth, windowHeight, "Rendering Engine", NULL, NULL);
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
    registerNewFramebufferSize(currWindow, windowWidth, windowHeight);

    // Setup shaders
    Shader defaultShader("Shaders/default.vert", "Shaders/default.frag");
    Shader framebufferShader("Shaders/framebuffer.vert", "Shaders/framebuffer.frag");
    /*
    // Textures
    Texture textures[] = {
        Texture("Textures/texWoodFloor.png", "diffuse", 0),
        Texture("Textures/texWoodFloorDisp.png", "specular", 1)
    }; */
    // Meshes //
    // Importing models
    Model model("Models/bunny/scene.gltf");

    // Model Translations
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f); //glm::vec3(0.5f, 0.5f, 0.5f);
    //glm::mat4 lightModel = glm::mat4(1.0f);
    //lightModel = glm::translate(lightModel, lightPos);

    // Uniforms
    double timePrev = 0.0;
    double timeCurr = 0.0;
    double timeDiff = 0.0;
    unsigned int counter = 0;
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");
    defaultShader.Activate();
    glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColour"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    framebufferShader.Activate();
    glUniform1i(glGetUniformLocation(framebufferShader.ID, "screenTexture"), 1); // bruh wtf it's supposed to be 0??
    glUniform1f(glGetUniformLocation(framebufferShader.ID, "screenWidth"), windowWidth);
    glUniform1f(glGetUniformLocation(framebufferShader.ID, "screenHeight"), windowHeight);

    // Camera
    Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    // Face culling
    glEnable(GL_DEPTH_TEST);

    // Frame Rect VAO VBO
    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVert), &frameVert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    FBO FBO;

    // Render loop
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        FBO.Bind();
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Depth testing ON before rendering frame buffer
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW); // CW or CCW for counter clockwise, depends on model

        // Camera
        camera.Inputs(currWindow);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Render meshes and models
        model.draw(defaultShader, camera);

        // Uniforms

        timeCurr = glfwGetTime();
        timeDiff = timeCurr - timePrev;
        counter++;
        if (timeDiff >= 1.0 / 30.0) {
            glfwSetWindowTitle(currWindow, (std::string("Rendering Engine | FPS: " + std::to_string((1.0 / timeDiff) * counter))).c_str());
            timePrev = timeCurr;
            counter = 0;
        }
        glUniform1f(uniTime, glfwGetTime());

        // Bind to FBO and compute post processing effects
        FBO.Unbind();
        framebufferShader.Activate();
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST); 
        glDisable(GL_CULL_FACE);
        FBO.BindTex();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    defaultShader.Delete();
    framebufferShader.Delete();
    FBO.Delete();
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
