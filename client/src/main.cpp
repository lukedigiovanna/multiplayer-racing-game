#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>

#include "utils/Shader.h"
#include "utils/Mesh.h"
#include "utils/input.h"

#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8080
#define MAXLINE 1024

#define YAW_BOUNDS M_PI / 2 - 0.05f

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::atomic<bool> gameIsActive = true;

void network_thread_func() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in serverAddress;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Error: socket creation failed\n";
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t len;

    // join game
    char msg[2];
    msg[0] = 0; // join game code
    msg[1] = '\0';
    sendto(sockfd, msg, 2, 0, (const sockaddr *) &serverAddress, sizeof(serverAddress));
    n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (sockaddr *) &serverAddress, &len);
    buffer[n] = '\0';
    std::cout << "Join request response: " << +buffer[0] << std::endl;

    while (gameIsActive) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (sockaddr *) &serverAddress, &len);
        float* playerData = (float*) buffer;
        std::cout << playerData[0] << ", " << playerData[1] << ", " << playerData[2] 
              << " : " << playerData[3] << ", " << playerData[4] << ", " << playerData[5] << "\n";
    }

    msg[0] = 1; // leave game code
    sendto(sockfd, msg, 2, 0, (const sockaddr *) &serverAddress, sizeof(serverAddress));

    close(sockfd);
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

    std::thread network_thread(network_thread_func);

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

    while (gameIsActive) {
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

        gameIsActive = !glfwWindowShouldClose(window);
    }

    glfwTerminate();

    network_thread.join();

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