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
    //Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

    /*
    // Textures
    Texture textures[] = {
        Texture("Textures/texWoodFloor.png", "diffuse", 0),
        Texture("Textures/texWoodFloorDisp.png", "specular", 1)
    };

    // Meshes //
    // Floor
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh floor(verts, ind, tex);
    
    // Repeat for Light VAO, VBO, EBO
    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    // Use same tex since we don't care about light textures
    Mesh light(lightVerts, lightInd, tex);
    */

    // Model Translations
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    /*
    glm::vec3 subjectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 subjectModel = glm::mat4(1.0f);
    subjectModel = glm::translate(subjectModel, subjectPos);
    */

    // Uniforms
    GLuint uniTime = glGetUniformLocation(defaultShader.ID, "iTime");
    /*
    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    */
    defaultShader.Activate();
    //glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(subjectModel));
    glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColour"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Camera
    Camera camera(WINDOWWIDTH, WINDOWHEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Importing models
    Model model("Models/bunny/scene.gltf");

    // Render loop
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(currWindow)) {
        handleUserInput(currWindow);

        // Rendering Commands
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Camera
        camera.Inputs(currWindow);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Render meshes and models
        //floor.Draw(defaultShader, camera);
		//light.draw(lightShader, camera, lightModel, lightPos, glm::quat(1.0f, -0.1f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        model.draw(defaultShader, camera);

        // Uniforms
        glUniform1f(uniTime, glfwGetTime());

        // Swap colour buffer & check if any events are triggered (e.g. keyboard or mouse input)
        glfwSwapBuffers(currWindow);
        glfwPollEvents();
    }

    // Cleanup
    defaultShader.Delete();
    //lightShader.Delete();
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
