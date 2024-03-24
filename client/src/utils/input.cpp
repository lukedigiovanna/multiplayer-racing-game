#include "input.h"

#include <iostream>

int keys[512];
int pressedKeys[512];

bool mouseDown_b = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        pressedKeys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
        pressedKeys[key] = false;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mouseDown_b = true;
    }
    else {
        mouseDown_b = false;
    }
}


void input::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    for (int i = 0; i < 256; i++) {
        keys[i] = false;
        pressedKeys[i] = false;
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

bool input::keyDown(int key) {
    return keys[key];
}

bool input::keyPressed(int key) {
    if (pressedKeys[key]) {
        pressedKeys[key] = false;
        return true;
    }
    return false;
}

bool input::mouseDown() {
    return mouseDown_b;
}