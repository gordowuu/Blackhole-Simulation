# External Dependencies Setup Guide

This guide will help you set up all required external libraries for the Black Hole Simulation project.

## Quick Setup (Automated)

### Windows (PowerShell)

Run this script from the project root:

```powershell
# Create external directory
New-Item -ItemType Directory -Force -Path external

# Download and extract GLFW
$glfwVersion = "3.3.9"
Invoke-WebRequest -Uri "https://github.com/glfw/glfw/releases/download/$glfwVersion/glfw-$glfwVersion.zip" -OutFile "external/glfw.zip"
Expand-Archive -Path "external/glfw.zip" -DestinationPath "external/glfw_temp"
Move-Item "external/glfw_temp/glfw-$glfwVersion" "external/glfw"

# Clone GLM
git clone --depth 1 https://github.com/g-truc/glm.git external/glm

# Clone ImGui
git clone --depth 1 https://github.com/ocornut/imgui.git external/imgui

# Download stb_image
New-Item -ItemType Directory -Force -Path external/stb
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" -OutFile "external/stb/stb_image.h"

Write-Host "Dependencies downloaded! Now generate GLAD..."
Write-Host "Visit https://glad.dav1d.de/ and generate with:"
Write-Host "  - Language: C/C++"
Write-Host "  - Specification: OpenGL"
Write-Host "  - gl Version: 4.6"
Write-Host "  - Profile: Core"
Write-Host "  - Generate a loader: YES"
Write-Host "Extract the zip to external/glad/"
```

### Linux

```bash
#!/bin/bash

# Install system packages
sudo apt-get update
sudo apt-get install -y git cmake build-essential

# Create external directory
mkdir -p external

# Install GLFW (system package)
sudo apt-get install -y libglfw3-dev

# Or build from source
cd external
git clone --depth 1 https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF
make -j$(nproc)
cd ../../..

# Clone GLM
cd external
git clone --depth 1 https://github.com/g-truc/glm.git
cd ..

# Clone ImGui
cd external
git clone --depth 1 https://github.com/ocornut/imgui.git
cd ..

# Download stb_image
mkdir -p external/stb
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -O external/stb/stb_image.h

echo "Dependencies installed! Generate GLAD at https://glad.dav1d.de/"
```

## Manual Setup

### 1. GLFW (Window Management)

**Option A: Download Pre-built** (Windows)
1. Go to https://www.glfw.org/download.html
2. Download the Windows pre-compiled binaries
3. Extract to `external/glfw/`

**Option B: Build from Source** (All Platforms)
```bash
cd external
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF
cmake --build . --config Release
```

### 2. GLAD (OpenGL Loader)

1. Visit https://glad.dav1d.de/
2. Configure:
   - **Language**: C/C++
   - **Specification**: OpenGL
   - **API gl**: Version 4.6
   - **Profile**: Core
   - Check "Generate a loader"
3. Click **Generate**
4. Download the zip file
5. Extract to `external/glad/`

Your structure should look like:
```
external/glad/
  ├── include/
  │   ├── glad/
  │   │   └── glad.h
  │   └── KHR/
  │       └── khrplatform.h
  └── src/
      └── glad.c
```

### 3. GLM (Mathematics)

```bash
cd external
git clone https://github.com/g-truc/glm.git
# GLM is header-only, no build required
```

Or download from: https://github.com/g-truc/glm/releases

### 4. ImGui (User Interface)

```bash
cd external
git clone https://github.com/ocornut/imgui.git
# ImGui will be compiled with the project
```

Or download from: https://github.com/ocornut/imgui/releases

### 5. stb_image (Image Loading)

```bash
mkdir -p external/stb
cd external/stb

# Download stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
```

Or manually download from: https://github.com/nothings/stb

## Verifying Installation

After setup, your `external/` directory should look like:

```
external/
├── glfw/
│   ├── include/
│   │   └── GLFW/
│   │       ├── glfw3.h
│   │       └── glfw3native.h
│   ├── src/
│   └── CMakeLists.txt
├── glad/
│   ├── include/
│   │   ├── glad/
│   │   │   └── glad.h
│   │   └── KHR/
│   │       └── khrplatform.h
│   └── src/
│       └── glad.c
├── glm/
│   ├── glm/
│   │   ├── glm.hpp
│   │   ├── gtc/
│   │   ├── gtx/
│   │   └── ...
│   └── CMakeLists.txt
├── imgui/
│   ├── imgui.h
│   ├── imgui.cpp
│   ├── imgui_draw.cpp
│   ├── imgui_tables.cpp
│   ├── imgui_widgets.cpp
│   ├── backends/
│   │   ├── imgui_impl_glfw.h
│   │   ├── imgui_impl_glfw.cpp
│   │   ├── imgui_impl_opengl3.h
│   │   └── imgui_impl_opengl3.cpp
│   └── ...
└── stb/
    └── stb_image.h
```

## Troubleshooting

### GLFW Build Issues

If GLFW fails to build, try:
```bash
# Install dependencies (Linux)
sudo apt-get install xorg-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Install dependencies (macOS)
brew install glfw
```

### GLAD Generation Issues

If GLAD website is down, use the glad-cli tool:
```bash
pip install glad
glad --api="gl=4.6" --out-path=external/glad --generator=c
```

### ImGui Backend Missing

Make sure you have both implementation files:
- `imgui_impl_glfw.cpp/h`
- `imgui_impl_opengl3.cpp/h`

These are in the `backends/` folder of the ImGui repository.

### GLM Not Found

Ensure GLM's CMakeLists.txt exists. If using a downloaded version, you may need to add:

```cmake
# In project CMakeLists.txt
include_directories(external/glm)
```

## Platform-Specific Notes

### Windows
- Use Visual Studio 2019 or later
- Make sure to select x64 architecture
- May need to install Windows SDK

### Linux
- Install OpenGL development files: `sudo apt-get install libgl1-mesa-dev`
- Install X11 development files: `sudo apt-get install libx11-dev`

### macOS
- Install Xcode Command Line Tools: `xcode-select --install`
- Use Homebrew for easier dependency management
- May need to set: `export MACOSX_DEPLOYMENT_TARGET=10.15`

## Alternative: Using vcpkg (Recommended for Windows)

```powershell
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install glfw3:x64-windows
.\vcpkg install glm:x64-windows
.\vcpkg install glad:x64-windows

# Integrate with CMake
.\vcpkg integrate install
```

Then in CMakeLists.txt:
```cmake
set(CMAKE_TOOLCHAIN_FILE "path/to/vcpkg/scripts/buildsystems/vcpkg.cmake")
```

## Getting Help

If you encounter issues:
1. Check the README.md for system requirements
2. Ensure you have OpenGL 4.6 capable drivers
3. Verify CMake version is 3.15+
4. Check compiler version meets C++17 requirements
5. Open an issue on GitHub with error logs

## License Information

All dependencies are open-source:
- GLFW: zlib/libpng license
- GLAD: MIT License
- GLM: MIT License
- ImGui: MIT License
- stb_image: MIT License / Public Domain
