# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-11-11

### Added
- Initial release of Black Hole Simulation
- Real-time Kerr black hole physics with frame dragging effects
- GPU-accelerated ray marching using OpenGL 4.6 compute shaders
- Interactive ImGui UI with parameter controls
- 5 black hole presets (Schwarzschild, Moderate Kerr, Near-Maximal Kerr, M87, Stellar)
- Logarithmic mass scaling for realistic size differences
- Advanced camera controls (orbit, pan, zoom)
- Keyboard shortcuts:
  - H: Toggle help window
  - ESC: Exit application
  - Q/W/E/R: Quality presets (Low/Medium/High/Ultra)
- Mouse wheel zoom support
- HDR rendering pipeline with ACES tone mapping
- Performance optimization: 60 FPS on RTX 5060
- Comprehensive documentation (README, QUICKSTART, PROJECT_SUMMARY)
- Automated setup scripts for Windows

### Features
- **Physics**:
  - Schwarzschild metric for non-rotating black holes
  - Kerr metric for rotating black holes (spin 0-0.998)
  - Frame dragging (Lense-Thirring effect)
  - Gravitational lensing with ray marching
  - Event horizon with spin-dependent radius
  - Photon sphere visualization
  - Accretion disk with temperature gradients

- **Rendering**:
  - GPU compute shader ray tracing
  - 500 ray marching steps (medium quality)
  - HDR tone mapping (ACES)
  - Bloom post-processing
  - Procedural starfield background
  - Real-time parameter updates

- **Controls**:
  - Left mouse drag: Orbit camera
  - Right mouse drag: Pan camera
  - Mouse wheel: Zoom
  - Middle mouse: Reset camera
  - Full keyboard shortcut support

- **Performance**:
  - Optimized for NVIDIA RTX GPUs
  - Multiple quality presets (250-2000 steps)
  - 60 FPS at 1920x1080 on RTX 5060
  - Efficient GPU memory usage

### Technical Details
- C++17 codebase
- OpenGL 4.6 Core profile
- CMake build system
- Cross-platform support (Windows, Linux, macOS)
- Dependencies: GLFW 3.3.9, GLM 1.1.0, ImGui 1.90.1, GLAD, stb_image

### Documentation
- Complete README with physics background
- Quick start guide for easy setup
- Project summary with technical details
- Contributing guidelines
- Setup instructions for all platforms

## [Unreleased]

### Planned Features
- Screenshot capture (F12)
- Accretion disk animation controls
- Time dilation visualization
- Multiple black holes in single scene
- VR/AR support
- Ray-traced shadows
- Relativistic jets
- Video recording capability
- Custom black hole parameter saving/loading
- Real-time physics parameter tweaking
- Educational mode with explanations
