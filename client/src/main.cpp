#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

#include "utils/Shader.h"
#include "utils/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int program() {
    std::cout << "Hello, from client\n";

    GLFWwindow* window;

    if (!glfwInit()) {
        throw std::runtime_error("ERROR: Failed to initialize GLFW");
    }

    window = glfwCreateWindow(640, 480, "Hello, World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("ERROR: Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    Shader shader("assets/shaders/vs.glsl", "assets/shader/fs.glsl");

    glm::mat4 projection = glm::perspective(90.0f, 640.0f / 480.0f, 0.1f, 1000.0f);

    VertexAttribSet attribSet = {
        {GL_FLOAT, 3, sizeof(float)},
        {GL_FLOAT, 3, sizeof(float)},
        {GL_FLOAT, 2, sizeof(float)}
    };
    float cubeVertices[288] = {
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

        // back face
        -0.5f, -0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

        0.5f,  0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

        // top face
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

        0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

        // bottom face
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

        0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,

        // right face
        0.5f, -0.5f,  -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,   0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,   0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,   0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        // left face
        -0.5f, -0.5f,  -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,   0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,   0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f,   0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  -0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    };

    Mesh cube(cubeVertices, 36, attribSet);

    glViewport(0, 0, 640, 480);
    
    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        shader.use();

        // glm::mat4 model(1.0f);
        // glm::mat4 view(1.0f);
        // view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // shader.setMatrix4("projection", projection);
        // shader.setMatrix4("view", view);
        // shader.setMatrix4("model", model);

        cube.render();
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

#ifdef _WIN32
    #include <windows.h>
    int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
        return program();
    }
#else
    int main(int argc, char* argv[]) {
        return program();
    }
#endif