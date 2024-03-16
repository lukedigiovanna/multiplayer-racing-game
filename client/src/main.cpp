#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

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

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

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