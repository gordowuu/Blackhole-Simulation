#include "Core/Window.h"
#include "Core/Camera.h"
#include "Core/Input.h"
#include "Physics/BlackHole.h"
#include "Physics/AccretionDisk.h"
#include "Physics/Constants.h"
#include "Rendering/Renderer.h"
#include "UI/Interface.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

int main() {
    try {
        // Create window
        Core::Window window(Physics::DEFAULT_WIDTH, Physics::DEFAULT_HEIGHT, 
                           "Black Hole Simulation - RTX Accelerated");
        
        // Initialize input system
        Core::Input::initialize(window.getHandle());
        
        // Set up callbacks
        window.setKeyCallback([](int key, int scancode, int action, int mods) {
            Core::Input::keyCallback(key, scancode, action, mods);
            
            // ESC to close
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                // Window will close on next frame
            }
        });
        
        window.setMouseButtonCallback([](int button, int action, int mods) {
            Core::Input::mouseButtonCallback(button, action, mods);
        });
        
        window.setCursorPosCallback([](double xpos, double ypos) {
            Core::Input::cursorPosCallback(xpos, ypos);
        });
        
        window.setScrollCallback([](double xoffset, double yoffset) {
            Core::Input::scrollCallback(xoffset, yoffset);
        });
        
        // Create camera
        Core::Camera camera(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f);
        
        // Create black hole (M87 black hole as default)
        Physics::BlackHole blackHole(Physics::DEFAULT_MASS, 0.9f);
        
        // Create accretion disk
        Physics::AccretionDisk disk(&blackHole);
        
        // Create renderer
        Rendering::Renderer renderer(window.getWidth(), window.getHeight());
        renderer.initialize();
        
        // Create UI
        UI::Interface ui(&window);
        
        // Set up window resize callback
        window.setResizeCallback([&](int width, int height) {
            renderer.resize(width, height);
        });
        
        // Main loop timing
        double lastTime = glfwGetTime();
        double deltaTime = 0.0;
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Black Hole Simulation Started" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  Left Mouse + Drag: Orbit camera" << std::endl;
        std::cout << "  Right Mouse + Drag: Pan camera" << std::endl;
        std::cout << "  Mouse Wheel: Zoom in/out" << std::endl;
        std::cout << "  Middle Mouse: Reset camera" << std::endl;
        std::cout << "  H: Toggle help window" << std::endl;
        std::cout << "  Q/W/E/R: Quality Low/Medium/High/Ultra" << std::endl;
        std::cout << "  ESC: Exit" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        std::cout << "Entering main loop..." << std::endl;
        int frameCount = 0;
        
        // Main render loop
        while (!window.shouldClose()) {
            frameCount++;
            if (frameCount <= 5) {
                std::cout << "Starting frame " << frameCount << "..." << std::endl;
            }
            
            // Calculate delta time
            double currentTime = glfwGetTime();
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            
            // Poll events
            window.pollEvents();
            Core::Input::update();
            
            if (frameCount == 1) {
                std::cout << "Frame 1: Input updated..." << std::endl;
            }
            
            // Update camera with input (if UI doesn't capture)
            ui.beginFrame();
            
            if (frameCount == 1) {
                std::cout << "Frame 1: UI frame began..." << std::endl;
            };
            
            if (!ui.wantsCaptureMouse()) {
                // Camera controls
                if (Core::Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
                    glm::vec2 delta = Core::Input::getMouseDelta();
                    camera.orbit(-delta.x * 0.005f, delta.y * 0.005f);
                }
                
                if (Core::Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
                    glm::vec2 delta = Core::Input::getMouseDelta();
                    camera.pan(glm::vec2(-delta.x * 0.01f, delta.y * 0.01f));
                }
                
                // Scroll wheel for zoom
                glm::vec2 scroll = Core::Input::getScrollDelta();
                if (scroll.y != 0.0f) {
                    camera.zoom(-scroll.y * 0.5f);
                }
                
                // Reset camera on middle mouse button
                if (Core::Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
                    camera.setPosition(glm::vec3(0.0f, 5.0f, 20.0f));
                    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
                }
            }
            
            // Keyboard controls (work even when UI captures mouse)
            if (!ui.wantsCaptureKeyboard()) {
                // Toggle help with H key
                if (Core::Input::isKeyPressed(GLFW_KEY_H)) {
                    ui.toggleHelp();
                }
                
                // Quality controls
                if (Core::Input::isKeyPressed(GLFW_KEY_Q)) {
                    renderer.setQuality(1); // Low
                    std::cout << "Quality: Low (fastest)" << std::endl;
                }
                if (Core::Input::isKeyPressed(GLFW_KEY_W)) {
                    renderer.setQuality(2); // Medium
                    std::cout << "Quality: Medium" << std::endl;
                }
                if (Core::Input::isKeyPressed(GLFW_KEY_E)) {
                    renderer.setQuality(3); // High
                    std::cout << "Quality: High" << std::endl;
                }
                if (Core::Input::isKeyPressed(GLFW_KEY_R)) {
                    renderer.setQuality(4); // Ultra
                    std::cout << "Quality: Ultra (slowest)" << std::endl;
                }
            }
            
            // ESC key to exit (always works)
            if (Core::Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
                std::cout << "ESC pressed, exiting..." << std::endl;
                glfwSetWindowShouldClose(window.getHandle(), GLFW_TRUE);
            }
            
            // Update camera
            camera.update(static_cast<float>(deltaTime));
            
            if (frameCount == 1) {
                std::cout << "Frame 1: Camera updated, starting render..." << std::endl;
            }
            
            // Render scene
            renderer.render(camera, blackHole, disk);
            
            if (frameCount == 1) {
                std::cout << "Frame 1: Render complete, rendering UI..." << std::endl;
            }
            
            // Render UI
            ui.renderControls(camera, blackHole, disk, renderer);
            ui.endFrame();
            
            if (frameCount == 1) {
                std::cout << "Frame 1: UI rendered, swapping buffers..." << std::endl;
            }
            
            // Swap buffers
            window.swapBuffers();
            
            if (frameCount == 1) {
                std::cout << "Frame 1: First frame complete!" << std::endl;
            }
            
            // Debug: Check if window wants to close
            if (frameCount <= 3 && window.shouldClose()) {
                std::cout << "WARNING: Window requested close after frame " << frameCount << std::endl;
            }
        }
        
        std::cout << "\nShutting down after " << frameCount << " frames..." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
