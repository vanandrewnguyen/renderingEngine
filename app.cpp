#include "app.hpp"

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800

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

Vertex vertices[] = {
    Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
};
GLuint indices[] = {  
    0, 1, 3, 
    1, 2, 3
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
    Shader raymarchingShader("Shaders/raymarch.vert", "Shaders/raymarch.frag");

    // Textures
    Texture textures[] = {
        Texture("Textures/texWoodFloor.png", "diffuse", 0),
        Texture("Textures/texWoodFloorDisp.png", "specular", 1)
    };
    // Meshes //
    // Raymarching rect map
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh screenMap(verts, ind, tex);
    
    // Model Translations
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f); //glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    // Uniforms
    double timePrev = 0.0;
    double timeCurr = 0.0;
    double timeDiff = 0.0;
    unsigned int counter = 0;
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");
    defaultShader.Activate();
    glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColour"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Camera
    Camera camera(WINDOWWIDTH, WINDOWHEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Importing models
    Model model("Models/bunny/scene.gltf");

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    // Face culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW); // CW or CCW for counter clockwise, depends on model

    // Render loop
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Camera
        camera.Inputs(currWindow);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Render meshes and models
        //screenMap.draw(raymarchingShader, camera); //save this for later
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

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    defaultShader.Delete();
    raymarchingShader.Delete();
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
