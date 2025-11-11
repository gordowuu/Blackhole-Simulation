#include "Interface.h"
#include "../Core/Window.h"
#include "../Core/Camera.h"
#include "../Physics/BlackHole.h"
#include "../Physics/AccretionDisk.h"
#include "../Rendering/Renderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace UI {

Interface::Interface(Core::Window* window)
    : m_lastFrameTime(0.0f)
    , m_frameCount(0)
    , m_fps(0.0f)
    , m_showHelp(true) {
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup style
    ImGui::StyleColorsDark();
    
    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

Interface::~Interface() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Interface::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Interface::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Interface::renderControls(Core::Camera& camera,
                               Physics::BlackHole& blackHole,
                               Physics::AccretionDisk& disk,
                               Rendering::Renderer& renderer) {
    // Main control window
    ImGui::Begin("Black Hole Simulation Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::CollapsingHeader("Black Hole", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderBlackHoleControls(blackHole);
    }
    
    if (ImGui::CollapsingHeader("Accretion Disk", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderAccretionDiskControls(disk);
    }
    
    if (ImGui::CollapsingHeader("Camera")) {
        renderCameraControls(camera);
    }
    
    if (ImGui::CollapsingHeader("Rendering")) {
        renderRenderingControls(renderer);
    }
    
    if (ImGui::CollapsingHeader("Presets")) {
        renderPresets(blackHole, disk, camera);
    }
    
    ImGui::Separator();
    renderPerformanceStats();
    
    ImGui::End();
    
    // Help window - only show if toggled on
    if (m_showHelp) {
        ImGui::Begin("Controls Help", &m_showHelp, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Camera Controls:");
        ImGui::BulletText("Left Mouse + Drag: Orbit camera");
        ImGui::BulletText("Right Mouse + Drag: Pan camera");
        ImGui::BulletText("Mouse Wheel: Zoom in/out");
        ImGui::BulletText("Middle Mouse: Reset camera");
        ImGui::Separator();
        ImGui::Text("Keyboard:");
        ImGui::BulletText("ESC: Exit application");
        ImGui::BulletText("H: Toggle this help window");
        ImGui::BulletText("F11: Toggle fullscreen");
        ImGui::BulletText("F12: Take screenshot");
        ImGui::Separator();
        ImGui::Text("Quality:");
        ImGui::BulletText("Q: Low quality (faster)");
        ImGui::BulletText("W: Medium quality");
        ImGui::BulletText("E: High quality");
        ImGui::BulletText("R: Ultra quality (slower)");
        ImGui::End();
    }
}

bool Interface::wantsCaptureMouse() const {
    return ImGui::GetIO().WantCaptureMouse;
}

bool Interface::wantsCaptureKeyboard() const {
    return ImGui::GetIO().WantCaptureKeyboard;
}

void Interface::renderBlackHoleControls(Physics::BlackHole& blackHole) {
    float mass = blackHole.getMass();
    float spin = blackHole.getSpin();
    
    if (ImGui::SliderFloat("Mass (Solar Masses)", &mass, 1.0f, 1000000.0f, "%.1f", ImGuiSliderFlags_Logarithmic)) {
        blackHole.setMass(mass);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Mass affects:");
        ImGui::BulletText("Size of black hole shadow");
        ImGui::BulletText("Gravitational lensing strength");
        ImGui::BulletText("Accretion disk dynamics");
        ImGui::EndTooltip();
    }
    
    if (ImGui::SliderFloat("Spin", &spin, 0.0f, 0.998f, "%.3f")) {
        blackHole.setSpin(spin);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Spin affects:");
        ImGui::BulletText("Frame dragging (spacetime rotation)");
        ImGui::BulletText("Event horizon size");
        ImGui::BulletText("Asymmetric light bending");
        ImGui::Text("\nSpin = 0: Schwarzschild (static)");
        ImGui::Text("Spin → 1: Kerr (rotating)");
        ImGui::EndTooltip();
    }
    
    ImGui::Separator();
    ImGui::Text("Physical Properties:");
    ImGui::BulletText("Schwarzschild Rs: %.2f", blackHole.getSchwarzschildRadius());
    ImGui::BulletText("Event Horizon: %.2f", blackHole.getEventHorizonRadius());
    ImGui::Text("Photon Sphere: %.2f", blackHole.getPhotonSphereRadius());
    ImGui::Text("ISCO: %.2f", blackHole.getISCO());
}

void Interface::renderAccretionDiskControls(Physics::AccretionDisk& disk) {
    float innerRadius = disk.getInnerRadius();
    float outerRadius = disk.getOuterRadius();
    float thickness = disk.getThickness();
    float inclination = disk.getInclination();
    float rotationSpeed = disk.getRotationSpeed();
    
    if (ImGui::SliderFloat("Inner Radius", &innerRadius, 1.0f, 50.0f)) {
        disk.setInnerRadius(innerRadius);
    }
    
    if (ImGui::SliderFloat("Outer Radius", &outerRadius, innerRadius + 1.0f, 100.0f)) {
        disk.setOuterRadius(outerRadius);
    }
    
    if (ImGui::SliderFloat("Thickness", &thickness, 0.1f, 5.0f)) {
        disk.setThickness(thickness);
    }
    
    if (ImGui::SliderFloat("Inclination", &inclination, 0.0f, 90.0f)) {
        disk.setInclination(glm::radians(inclination));
    }
    
    if (ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 2.0f)) {
        disk.setRotationSpeed(rotationSpeed);
    }
}

void Interface::renderCameraControls(Core::Camera& camera) {
    float distance = camera.getDistance();
    float fov = camera.getFOV();
    
    if (ImGui::SliderFloat("Distance", &distance, 1.0f, 100.0f)) {
        camera.setDistance(distance);
    }
    
    if (ImGui::SliderFloat("Field of View", &fov, 30.0f, 120.0f)) {
        camera.setFOV(fov);
    }
    
    if (ImGui::Button("Reset Camera")) {
        camera.setPosition(glm::vec3(0.0f, 5.0f, 20.0f));
        camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
        camera.setFOV(60.0f);
    }
}

void Interface::renderRenderingControls(Rendering::Renderer& renderer) {
    int quality = renderer.getQuality();
    bool enableBloom = renderer.getEnableBloom();
    float exposure = renderer.getExposure();
    bool showEventHorizon = renderer.getShowEventHorizon();
    bool showPhotonSphere = renderer.getShowPhotonSphere();
    bool showAccretionDisk = renderer.getShowAccretionDisk();
    
    // Quality selector with keyboard hint
    const char* qualityLevels[] = { "Low (Q)", "Medium (W)", "High (E)", "Ultra (R)" };
    int displayQuality = quality - 1; // Convert 1-4 to 0-3 for combo box
    if (displayQuality < 0) displayQuality = 0;
    if (displayQuality > 3) displayQuality = 3;
    
    if (ImGui::Combo("Quality", &displayQuality, qualityLevels, 4)) {
        renderer.setQuality(displayQuality + 1); // Convert back to 1-4
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Ray marching quality:");
        ImGui::BulletText("Low: 250 steps (fastest)");
        ImGui::BulletText("Medium: 500 steps (balanced)");
        ImGui::BulletText("High: 1000 steps (detailed)");
        ImGui::BulletText("Ultra: 2000 steps (slowest)");
        ImGui::EndTooltip();
    }
    
    if (ImGui::Checkbox("Enable Bloom", &enableBloom)) {
        renderer.setEnableBloom(enableBloom);
    }
    
    if (ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.0f)) {
        renderer.setExposure(exposure);
    }
    
    ImGui::Separator();
    ImGui::Text("Visualization:");
    
    if (ImGui::Checkbox("Show Event Horizon", &showEventHorizon)) {
        renderer.setShowEventHorizon(showEventHorizon);
    }
    
    if (ImGui::Checkbox("Show Photon Sphere", &showPhotonSphere)) {
        renderer.setShowPhotonSphere(showPhotonSphere);
    }
    
    if (ImGui::Checkbox("Show Accretion Disk", &showAccretionDisk)) {
        renderer.setShowAccretionDisk(showAccretionDisk);
    }
}

void Interface::renderPresets(Physics::BlackHole& blackHole, 
                              Physics::AccretionDisk& disk,
                              Core::Camera& camera) {
    ImGui::Text("Black Hole Presets:");
    ImGui::Separator();
    
    if (ImGui::Button("Schwarzschild (Non-rotating)", ImVec2(-1, 0))) {
        blackHole.setMass(10.0f);
        blackHole.setSpin(0.0f);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Classic non-rotating black hole");
        ImGui::Text("Mass: 10 M☉, Spin: 0.0");
        ImGui::EndTooltip();
    }
    
    if (ImGui::Button("Moderate Kerr", ImVec2(-1, 0))) {
        blackHole.setMass(10.0f);
        blackHole.setSpin(0.5f);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Moderate rotation with frame dragging");
        ImGui::Text("Mass: 10 M☉, Spin: 0.5");
        ImGui::EndTooltip();
    }
    
    if (ImGui::Button("Near-Maximal Kerr", ImVec2(-1, 0))) {
        blackHole.setMass(10.0f);
        blackHole.setSpin(0.95f);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Extreme rotation - maximum spin allowed");
        ImGui::Text("Mass: 10 M☉, Spin: 0.95");
        ImGui::Text("Strong frame dragging effects!");
        ImGui::EndTooltip();
    }
    
    if (ImGui::Button("Supermassive (M87)", ImVec2(-1, 0))) {
        blackHole.setMass(6.5e9f);
        blackHole.setSpin(0.9f);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("The real M87* black hole");
        ImGui::Text("Mass: 6.5 billion M☉, Spin: 0.9");
        ImGui::Text("First black hole ever photographed!");
        ImGui::EndTooltip();
    }
    
    if (ImGui::Button("Stellar Mass", ImVec2(-1, 0))) {
        blackHole.setMass(5.0f);
        blackHole.setSpin(0.7f);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Typical stellar-mass black hole");
        ImGui::Text("Mass: 5 M☉, Spin: 0.7");
        ImGui::EndTooltip();
    }
}

void Interface::renderPerformanceStats() {
    ImGui::Separator();
    ImGui::Text("Performance:");
    float fps = ImGui::GetIO().Framerate;
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / fps);
    
    // Add color-coded performance indicator
    if (fps >= 60.0f) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Excellent");
    } else if (fps >= 30.0f) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Status: Good");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: Poor");
    }
}

} // namespace UI
