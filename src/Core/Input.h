#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

namespace Core {

class Input {
public:
    static void initialize(GLFWwindow* window);
    static void update();
    
    // Keyboard
    static bool isKeyPressed(int key);
    static bool isKeyDown(int key);
    static bool isKeyReleased(int key);
    
    // Mouse
    static bool isMouseButtonPressed(int button);
    static bool isMouseButtonDown(int button);
    static bool isMouseButtonReleased(int button);
    
    static glm::vec2 getMousePosition();
    static glm::vec2 getMouseDelta();
    static glm::vec2 getScrollDelta();
    
    static void setMousePosition(const glm::vec2& position);
    static void setCursorMode(bool visible);
    
    // Callback functions (public to allow lambda forwarding from main)
    static void keyCallback(int key, int scancode, int action, int mods);
    static void mouseButtonCallback(int button, int action, int mods);
    static void cursorPosCallback(double xpos, double ypos);
    static void scrollCallback(double xoffset, double yoffset);
    
private:
    friend class Window;
    
    static GLFWwindow* s_window;
    
    static bool s_keys[512];
    static bool s_keysLast[512];
    static bool s_mouseButtons[8];
    static bool s_mouseButtonsLast[8];
    
    static glm::vec2 s_mousePos;
    static glm::vec2 s_mouseLastPos;
    static glm::vec2 s_mouseDelta;
    static glm::vec2 s_scrollDelta;
    static bool s_firstMouse;
};

} // namespace Core
