#pragma once

#include <GLFW/glfw3.h>

namespace input {
    void init(GLFWwindow* window);

    bool keyDown(int key);
    bool keyPressed(int key);

    bool mouseDown();
}