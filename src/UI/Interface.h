#pragma once

namespace Core {
    class Window;
    class Camera;
}

namespace Physics {
    class BlackHole;
    class AccretionDisk;
}

namespace Rendering {
    class Renderer;
}

namespace UI {

class Interface {
public:
    Interface(Core::Window* window);
    ~Interface();
    
    void beginFrame();
    void endFrame();
    
    void renderControls(Core::Camera& camera,
                       Physics::BlackHole& blackHole,
                       Physics::AccretionDisk& disk,
                       Rendering::Renderer& renderer);
    
    bool wantsCaptureMouse() const;
    bool wantsCaptureKeyboard() const;
    
    void toggleHelp() { m_showHelp = !m_showHelp; }
    
private:
    void renderBlackHoleControls(Physics::BlackHole& blackHole);
    void renderAccretionDiskControls(Physics::AccretionDisk& disk);
    void renderCameraControls(Core::Camera& camera);
    void renderRenderingControls(Rendering::Renderer& renderer);
    void renderPresets(Physics::BlackHole& blackHole, 
                      Physics::AccretionDisk& disk,
                      Core::Camera& camera);
    void renderPerformanceStats();
    
    float m_lastFrameTime;
    int m_frameCount;
    float m_fps;
    bool m_showHelp;
};

} // namespace UI
