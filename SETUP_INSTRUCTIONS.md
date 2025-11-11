# Black Hole Simulation - Complete Setup Instructions

## Prerequisites

Before you begin, ensure you have:
1. **Git** - Download from https://git-scm.com/
2. **CMake 3.15+** - Download from https://cmake.org/download/
3. **Visual Studio 2019/2022** - With C++ Desktop Development workload
4. **OpenGL 4.6 capable GPU** - NVIDIA RTX or equivalent

## Step-by-Step Setup

### 1. Clone or Download the Project

If you haven't already:
```powershell
git clone https://github.com/yourusername/BlackholeSim.git
cd BlackholeSim
```

### 2. Install External Dependencies

The project requires these libraries:

#### A. GLFW (Window Management)
```powershell
cd external
git clone --depth 1 --branch 3.3.9 https://github.com/glfw/glfw.git
cd ..
```

#### B. GLM (Mathematics Library)
```powershell
cd external
git clone --depth 1 https://github.com/g-truc/glm.git
cd ..
```

#### C. ImGui (User Interface)
```powershell
cd external
git clone --depth 1 --branch v1.90.1 https://github.com/ocornut/imgui.git
cd ..
```

#### D. stb_image (Image Loading)
```powershell
# Already downloaded by script, or:
mkdir external\stb
curl -o external\stb\stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
```

#### E. GLAD (OpenGL Loader) - **MANUAL STEP REQUIRED**

GLAD must be generated specifically for your needs:

1. Visit **https://glad.dav1d.de/**

2. Configure the generator:
   - **Language**: C/C++
   - **Specification**: OpenGL
   - **API gl**: Version **4.6**
   - **Profile**: **Core**
   - **Options**: Check "Generate a loader"

3. Click **GENERATE**

4. Download the generated `glad.zip` file

5. Extract it to `external/glad/` with this structure:
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

### 3. Verify Directory Structure

Your `external/` folder should now look like:

```
external/
├── glad/
│   ├── include/
│   │   ├── glad/
│   │   │   └── glad.h
│   │   └── KHR/
│   │       └── khrplatform.h
│   └── src/
│       └── glad.c
├── glfw/
│   ├── include/
│   ├── src/
│   └── CMakeLists.txt
├── glm/
│   ├── glm/
│   │   └── glm.hpp
│   └── CMakeLists.txt
├── imgui/
│   ├── imgui.h
│   ├── imgui.cpp
│   ├── backends/
│   │   ├── imgui_impl_glfw.cpp
│   │   ├── imgui_impl_glfw.h
│   │   ├── imgui_impl_opengl3.cpp
│   │   └── imgui_impl_opengl3.h
│   └── [other imgui files]
└── stb/
    └── stb_image.h
```

### 4. Build the Project

#### Option A: Using build.ps1 Script (Recommended)

```powershell
.\build.ps1
```

This script will:
- Check for dependencies
- Generate Visual Studio solution
- Build the project
- Offer to launch the application

#### Option B: Manual CMake Build

```powershell
# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake .. -G "Visual Studio 17 2022" -A x64

# Build Release configuration
cmake --build . --config Release

# Run the executable
.\bin\Release\BlackholeSim.exe
```

### 5. Run the Application

After successful build:

```powershell
# From project root:
.\build\bin\Release\BlackholeSim.exe

# Or from build directory:
cd build\bin\Release
.\BlackholeSim.exe
```

## Troubleshooting

### "CMake Error: Could not find GLFW"

**Solution**: Ensure GLFW is cloned into `external/glfw/` with its CMakeLists.txt present

### "Cannot find glad/glad.h"

**Solution**: Generate GLAD from https://glad.dav1d.de/ and extract to `external/glad/`

### "OpenGL 4.6 not supported"

**Solution**: 
- Update your GPU drivers
- Check if your GPU supports OpenGL 4.6: run `glxinfo` (Linux) or use GPU-Z (Windows)

### "LNK2019: unresolved external symbol" errors

**Solution**: Ensure all source files are included in CMakeLists.txt and rebuild

### Build succeeds but application crashes on startup

**Solution**:
1. Ensure shaders/ folder is copied to build directory (CMakeLists.txt should do this)
2. Check if shaders/raytracer.comp exists
3. Run from correct directory (where shaders/ and assets/ folders are accessible)

### "GLFW window creation failed"

**Solution**:
- Check if OpenGL drivers are installed
- Try running as administrator
- Update graphics drivers

## Testing the Build

Once running, you should see:
1. A window titled "Black Hole Simulation - RTX Accelerated"
2. A black hole with gravitational lensing effects
3. An accretion disk (if enabled)
4. ImGui control panel on the left

### Quick Test:
- Move mouse while holding left button → Camera should orbit
- Scroll mouse wheel → Camera should zoom
- Adjust "Spin" slider in UI → Black hole should change behavior

## Performance Expectations

On your RTX 5060 @ 1080p, expect:
- **120+ FPS** on Medium quality
- **95+ FPS** on High quality
- **60+ FPS** on Ultra quality

If performance is lower:
1. Lower quality setting
2. Reduce window resolution
3. Disable bloom effect
4. Check GPU is not thermal throttling

## Next Steps

Once built successfully:
1. Read README.md for usage instructions
2. Experiment with presets in the UI
3. Adjust physics parameters
4. Try different camera angles

## Alternative Setup Methods

### Using vcpkg (Advanced)

```powershell
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install glfw3:x64-windows glm:x64-windows

# In CMakeLists.txt, set:
# set(CMAKE_TOOLCHAIN_FILE "path/to/vcpkg/scripts/buildsystems/vcpkg.cmake")
```

### Manual Library Management

If you prefer to manage libraries differently:
1. Download pre-built binaries for GLFW
2. Place headers in `external/[lib]/include/`
3. Place libraries in `external/[lib]/lib/`
4. Update CMakeLists.txt accordingly

## Getting Help

If you encounter issues:

1. Check external/README.md for detailed dependency instructions
2. Verify all dependencies are correctly installed
3. Check CMake output for specific errors
4. Ensure OpenGL 4.6 support with latest drivers
5. Open an issue on GitHub with:
   - Your system specs
   - CMake version (`cmake --version`)
   - Full error output
   - Steps you've tried

## System Information

To help with debugging, gather:
```powershell
# GPU Info
wmic path win32_VideoController get name,driverVersion

# CMake version
cmake --version

# Visual Studio version
devenv /?

# OpenGL version (after building)
# Run the app, it prints OpenGL info on startup
```

## Success Indicators

You've successfully built when:
- ✓ CMake completes without errors
- ✓ Build completes without link errors  
- ✓ Executable runs without crashing
- ✓ Black hole renders on screen
- ✓ UI is responsive
- ✓ Camera controls work
- ✓ FPS counter shows reasonable values

## Common Issues Summary

| Issue | Solution |
|-------|----------|
| Git clone fails | Download zips manually from GitHub |
| GLAD missing | Generate at glad.dav1d.de |
| CMake can't find libs | Check external/ structure |
| Link errors | Rebuild clean (`rm -rf build`) |
| Crashes on start | Check shader files copied |
| Black screen | Update GPU drivers |
| Low FPS | Lower quality setting |

---

**Still having issues? Check external/README.md for more detailed instructions!**
