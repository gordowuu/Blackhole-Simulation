#pragma once

#include <string>
#include <functional>

struct GLFWwindow;

namespace Core {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Prevent copying
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    
    GLFWwindow* getHandle() const { return m_window; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
    
    void setTitle(const std::string& title);
    void setVSync(bool enabled);
    
    // Callbacks
    void setResizeCallback(std::function<void(int, int)> callback) { m_resizeCallback = callback; }
    void setKeyCallback(std::function<void(int, int, int, int)> callback) { m_keyCallback = callback; }
    void setMouseButtonCallback(std::function<void(int, int, int)> callback) { m_mouseButtonCallback = callback; }
    void setCursorPosCallback(std::function<void(double, double)> callback) { m_cursorPosCallback = callback; }
    void setScrollCallback(std::function<void(double, double)> callback) { m_scrollCallback = callback; }

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* m_window;
    int m_width;
    int m_height;
    
    std::function<void(int, int)> m_resizeCallback;
    std::function<void(int, int, int, int)> m_keyCallback;
    std::function<void(int, int, int)> m_mouseButtonCallback;
    std::function<void(double, double)> m_cursorPosCallback;
    std::function<void(double, double)> m_scrollCallback;
};

} // namespace Core
