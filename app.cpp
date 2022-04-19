#include "app.hpp"


// Verts and Inds for Floor Mesh
float floorSize = 0.25f;
Vertex vertices[] = {
    // Vert Coord // Normals // Colour // Tex Coord
    Vertex{glm::vec3(-floorSize, 0.0f,  floorSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-floorSize, 0.0f, -floorSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(floorSize, 0.0f, -floorSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(floorSize, 0.0f,  floorSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
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
    Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    // Textures
    Texture textures[] = {
        Texture("Textures/texWoodFloor.png", "diffuse", 0),
        Texture("Textures/texWoodFloorDisp.png", "specular", 1)
    }; 
    
    // Model Translations
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f); 

    // Uniforms
    double timePrev = 0.0;
    double timeCurr = 0.0;
    double timeDiff = 0.0;
    unsigned int counter = 0;
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");
    defaultShader.Activate();
    glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColour"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "skyboxTex"), 1);
    framebufferShader.Activate();
    glUniform1i(glGetUniformLocation(framebufferShader.ID, "screenTexture"), 1); // bruh wtf it's supposed to be 0??
    glUniform1f(glGetUniformLocation(framebufferShader.ID, "screenWidth"), windowWidth);
    glUniform1f(glGetUniformLocation(framebufferShader.ID, "screenHeight"), windowHeight);
    skyboxShader.Activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skyboxTex"), 1);

    // Materials
    Material matFloor(0.0f, 1.0f, 0.0f, { 1.0, 1.0, 1.0 }, defaultShader);
    Material matDiffuse(0.0f, 1.0f, 0.0f, { 1.0, 1.0, 1.0 }, defaultShader);
    Material matGlass(0.0f, 1.33f, 1.0f, { 1.0, 1.0, 1.0 }, defaultShader);
    Material matMetal(1.0f, 1.0f, 0.0f, { 1.0, 1.0, 1.0 }, defaultShader);
    
    // Load models and meshes
    Model modelBunny("Models/bunny/scene.gltf");
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh meshFloor(verts, ind, tex);

    // Camera
    Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    // Face culling
    glEnable(GL_DEPTH_TEST);

    // Init frame buffer (arg is for determining whether we apply MSAA or not)
    FBO postProcessingFBO(false);
    FBO antiAliasFBO(true);

    // Init cubemap for skybox
    Cubemap Skybox("ExteriorClouds");

    // Render loop
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        antiAliasFBO.Bind();
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
        matGlass.refreshMaterialProperties(defaultShader);
        modelBunny.draw(defaultShader, camera, matGlass, std::vector<float> {0.15f, 0.0f, 0.0f});
        matMetal.refreshMaterialProperties(defaultShader);
        modelBunny.draw(defaultShader, camera, matMetal, std::vector<float> {-0.15f, 0.0f, 0.0f});
        matDiffuse.refreshMaterialProperties(defaultShader);
        modelBunny.draw(defaultShader, camera, matDiffuse, std::vector<float> {0.0f, 0.0f, 0.0f});
        // Disable culling since mesh will be destroyed otherwise
        glDisable(GL_CULL_FACE);
        matFloor.refreshMaterialProperties(defaultShader);
        meshFloor.draw(defaultShader, camera, matFloor, std::vector<float> {0.0f, 0.062f, 0.0f});

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
        glUniform3f(glGetUniformLocation(framebufferShader.ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(framebufferShader.ID, "cameraOrientation"), camera.Orientation.x, camera.Orientation.y, camera.Orientation.z);

        // Drawing the skybox
        Skybox.draw(windowWidth, windowHeight, skyboxShader, camera);

        // Switch fbo's 
        glBindFramebuffer(GL_READ_FRAMEBUFFER, antiAliasFBO.FBOID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO.FBOID);
        glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Bind to FBO and compute post processing effects
        antiAliasFBO.Unbind();
        framebufferShader.Activate();
        glBindVertexArray(antiAliasFBO.rectVAOID);
        glDisable(GL_DEPTH_TEST); 
        glDisable(GL_CULL_FACE);
        postProcessingFBO.BindTex();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    defaultShader.Delete();
    framebufferShader.Delete();
    skyboxShader.Delete();
    postProcessingFBO.Delete();
    antiAliasFBO.Delete();
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

/*
float timeStagger = 2.0;
if (int(glfwGetTime() / timeStagger) % 3 == 0) {
    model.draw(defaultShader, camera, matDiffuse);
    matDiffuse.refreshMaterialProperties(defaultShader);
} else if (int(glfwGetTime() / timeStagger) % 3 == 1) {
    model.draw(defaultShader, camera, matGlass);
    matGlass.refreshMaterialProperties(defaultShader);
} else {
    model.draw(defaultShader, camera, matMetal);
    matMetal.refreshMaterialProperties(defaultShader);
}
*/