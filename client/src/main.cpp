#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>

#include "utils/Shader.h"
#include "utils/Mesh.h"
#include "utils/input.h"

#define YAW_BOUNDS M_PI / 2 - 0.05f

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int program() {
    GLFWwindow* window;

    if (!glfwInit()) {
        throw std::runtime_error("ERROR: Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Racer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("ERROR: Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    input::init(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    Shader shader = Shader("assets/shaders/vs.glsl", "assets/shaders/fs.glsl");
    shader.use();

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

    glm::vec3 cameraPosition(-5.0f, 0.0f, 0.0f);
    float rotation = 0.0f, yaw = 0.0f;

    double lastMouseX, lastMouseY;
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);

    bool focused = false;

    while (!glfwWindowShouldClose(window)) {
        if (focused && input::keyPressed(GLFW_KEY_ESCAPE)) {
            focused = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (!focused && input::mouseDown()) {
            focused = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        if (focused) {
            rotation += (mouseX - lastMouseX) / 50.0f;
            yaw += (lastMouseY - mouseY) / 50.0f;
            if (yaw < -YAW_BOUNDS) yaw = -YAW_BOUNDS;
            if (yaw > YAW_BOUNDS) yaw = YAW_BOUNDS;
        }

        glm::vec3 cameraForward = glm::normalize(glm::vec3(std::cosf(rotation), std::sinf(yaw), std::sinf(rotation)));
        glm::vec3 forward(std::cosf(rotation), 0.0f, std::sinf(rotation));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::cross(forward, up);

        if (input::keyDown(GLFW_KEY_W)) {
            cameraPosition += forward * 0.05f;
        }
        if (input::keyDown(GLFW_KEY_S)) {
            cameraPosition -= forward * 0.05f;
        }
        if (input::keyDown(GLFW_KEY_D)) {
            cameraPosition += right * 0.05f;
        }
        if (input::keyDown(GLFW_KEY_A)) {
            cameraPosition -= right * 0.05f;
        }
        if (input::keyDown(GLFW_KEY_SPACE)) {
            cameraPosition += up * 0.05f;
        }
        if (input::keyDown(GLFW_KEY_LEFT_SHIFT)) {
            cameraPosition -= up * 0.05f;
        }
        

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMatrix4("projection", projection);
        shader.setMatrix4("view", view);
        shader.setMatrix4("model", model);

        cube.render();
        
        glfwSwapBuffers(window);

        glfwPollEvents();

        lastMouseX = mouseX;
        lastMouseY = mouseY;
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