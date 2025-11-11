#include "Input.h"
#include <GLFW/glfw3.h>
#include <cstring>

namespace Core {

GLFWwindow* Input::s_window = nullptr;
bool Input::s_keys[512] = {};
bool Input::s_keysLast[512] = {};
bool Input::s_mouseButtons[8] = {};
bool Input::s_mouseButtonsLast[8] = {};
glm::vec2 Input::s_mousePos = glm::vec2(0.0f);
glm::vec2 Input::s_mouseLastPos = glm::vec2(0.0f);
glm::vec2 Input::s_mouseDelta = glm::vec2(0.0f);
glm::vec2 Input::s_scrollDelta = glm::vec2(0.0f);
bool Input::s_firstMouse = true;

void Input::initialize(GLFWwindow* window) {
    s_window = window;
    std::memset(s_keys, 0, sizeof(s_keys));
    std::memset(s_keysLast, 0, sizeof(s_keysLast));
    std::memset(s_mouseButtons, 0, sizeof(s_mouseButtons));
    std::memset(s_mouseButtonsLast, 0, sizeof(s_mouseButtonsLast));
}

void Input::update() {
    // Update previous states
    std::memcpy(s_keysLast, s_keys, sizeof(s_keys));
    std::memcpy(s_mouseButtonsLast, s_mouseButtons, sizeof(s_mouseButtons));
    
    // Reset deltas
    s_scrollDelta = glm::vec2(0.0f);
}

bool Input::isKeyPressed(int key) {
    if (key < 0 || key >= 512) return false;
    return s_keys[key] && !s_keysLast[key];
}

bool Input::isKeyDown(int key) {
    if (key < 0 || key >= 512) return false;
    return s_keys[key];
}

bool Input::isKeyReleased(int key) {
    if (key < 0 || key >= 512) return false;
    return !s_keys[key] && s_keysLast[key];
}

bool Input::isMouseButtonPressed(int button) {
    if (button < 0 || button >= 8) return false;
    return s_mouseButtons[button] && !s_mouseButtonsLast[button];
}

bool Input::isMouseButtonDown(int button) {
    if (button < 0 || button >= 8) return false;
    return s_mouseButtons[button];
}

bool Input::isMouseButtonReleased(int button) {
    if (button < 0 || button >= 8) return false;
    return !s_mouseButtons[button] && s_mouseButtonsLast[button];
}

glm::vec2 Input::getMousePosition() {
    return s_mousePos;
}

glm::vec2 Input::getMouseDelta() {
    return s_mouseDelta;
}

glm::vec2 Input::getScrollDelta() {
    return s_scrollDelta;
}

void Input::setMousePosition(const glm::vec2& position) {
    glfwSetCursorPos(s_window, position.x, position.y);
    s_mousePos = position;
}

void Input::setCursorMode(bool visible) {
    glfwSetInputMode(s_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Input::keyCallback(int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 512) {
        if (action == GLFW_PRESS) {
            s_keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            s_keys[key] = false;
        }
    }
}

void Input::mouseButtonCallback(int button, int action, int mods) {
    if (button >= 0 && button < 8) {
        if (action == GLFW_PRESS) {
            s_mouseButtons[button] = true;
        } else if (action == GLFW_RELEASE) {
            s_mouseButtons[button] = false;
        }
    }
}

void Input::cursorPosCallback(double xpos, double ypos) {
    glm::vec2 newPos(static_cast<float>(xpos), static_cast<float>(ypos));
    
    if (s_firstMouse) {
        s_mouseLastPos = newPos;
        s_firstMouse = false;
    }
    
    s_mouseDelta = newPos - s_mouseLastPos;
    s_mouseLastPos = s_mousePos;
    s_mousePos = newPos;
}

void Input::scrollCallback(double xoffset, double yoffset) {
    s_scrollDelta = glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

} // namespace Core
