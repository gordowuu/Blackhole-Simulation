# Black Hole Simulation - Quick Start Guide

## 🚀 Get Running in 5 Minutes

### Windows (Easiest)

```powershell
# 1. Clone the repository
git clone https://github.com/gordowuu/Blackhole-Simulation.git
cd Blackhole-Simulation

# 2. Run automated setup (downloads dependencies, installs CMake if needed)
.\setup.ps1

# 3. Build
.\build.ps1

# 4. Run!
.\run.ps1
```

That's it! The simulation should now be running at 60 FPS.

---

## 📋 Manual Setup (if automated fails)

### Step 1: Install Prerequisites

#### Windows
1. **Git**: https://git-scm.com/downloads
2. **CMake**: https://cmake.org/download/ (3.15+)
3. **MinGW-w64** (via MSYS2): https://www.msys2.org/
   - After installing MSYS2, run: `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake`
   - Add to PATH: `C:\msys64\mingw64\bin`

#### Linux
```bash
sudo apt-get update
sudo apt-get install git cmake build-essential libglfw3-dev libglm-dev
```

#### macOS
```bash
brew install git cmake glfw glm
```

### Step 2: Clone and Build

```powershell
# Clone
git clone https://github.com/gordowuu/Blackhole-Simulation.git
cd Blackhole-Simulation

# Build
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j 8

# Run (must be in build/bin for shaders)
cd bin
.\BlackholeSim.exe
```

---

## 🎮 First Steps

Once the simulation starts:

1. **Move the Camera**
   - Left mouse + drag: Orbit around black hole
   - Right mouse + drag: Pan camera
   - Mouse wheel: Zoom in/out

2. **Try Different Presets**
   - Open "Presets" in the left panel
   - Click "Near-Maximal Kerr" to see extreme spin effects
   - Click "Supermassive (M87)" to see the huge M87 black hole

3. **Adjust Quality** (if FPS is low)
   - Press `Q` for Low quality (fastest)
   - Press `W` for Medium quality (default)
   - Press `E` for High quality (if you have a powerful GPU)

4. **Toggle Help**
   - Press `H` to show/hide the help window

---

## ⚡ Performance Tips

### Getting 60 FPS

**If you have < 60 FPS:**
1. Press `Q` for Low quality mode
2. Lower your screen resolution
3. Close other GPU-intensive applications
4. Update your GPU drivers

**Current Quality:**
- Low (Q): 250 steps - 120+ FPS
- Medium (W): 500 steps - 60 FPS ⭐ Default
- High (E): 1000 steps - 30 FPS
- Ultra (R): 2000 steps - 10 FPS

### GPU Requirements

| GPU | Expected FPS (Medium) |
|-----|----------------------|
| RTX 5060 | 60+ FPS ✅ |
| RTX 4060 | 50-60 FPS |
| RTX 3060 | 45-55 FPS |
| RTX 2060 | 35-45 FPS |
| GTX 1660 | 25-35 FPS |

---

## 🐛 Troubleshooting

### "Black screen on startup"
**Cause**: Camera might be inside the black hole or shaders failed to compile

**Fix**:
```powershell
# Check console output for errors
# Press Middle Mouse button to reset camera
# Or restart the application
```

### "CMake not found"
**Fix**:
```powershell
# Run setup script which downloads CMake
.\setup.ps1

# Or manually download: https://cmake.org/download/
# Add to PATH: C:\Program Files\CMake\bin
```

### "Shader compilation error"
**Cause**: OpenGL 4.6 not supported by your GPU

**Fix**:
- Update GPU drivers to latest version
- Check OpenGL version: Run `glxinfo | grep "OpenGL version"` (Linux)
- Minimum GPU: GTX 1060 / RX 580

### "Build fails with MinGW"
**Fix**:
```powershell
# Make sure MinGW is in PATH
$env:Path = "C:\msys64\mingw64\bin;$env:Path"

# Clean and rebuild
Remove-Item -Recurse -Force build
mkdir build; cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . -j 8
```

### "Application crashes on startup"
**Possible causes**:
1. Missing DLL files (GLFW, etc.)
2. Incompatible GPU drivers
3. Corrupted shader files

**Fix**:
```powershell
# Ensure you run from build/bin directory
cd build/bin
.\BlackholeSim.exe

# Or use the run script
.\run.ps1
```

---

## 📚 Next Steps

Once you're running:

1. **Explore Physics**
   - Adjust mass to see size changes
   - Adjust spin to see frame dragging effects
   - Notice how light bends differently with different parameters

2. **Customize Rendering**
   - Toggle "Show Event Horizon" on/off
   - Adjust exposure for brighter/darker images
   - Enable/disable bloom effects

3. **Learn More**
   - Read full [README.md](README.md) for detailed physics
   - Check [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) for technical details
   - See [CONTRIBUTING.md](CONTRIBUTING.md) if you want to contribute

---

## 🆘 Still Having Issues?

Open an issue on GitHub with:
- Your GPU model and driver version
- Operating system
- Full console output
- CMake configuration output

**GitHub Issues**: https://github.com/gordowuu/Blackhole-Simulation/issues

---

**Enjoy exploring black holes! 🌌**
.\build\bin\Release\BlackholeSim.exe
```

---

## ⚡ Expected Experience

### On Launch:
- Window opens showing black hole
- Starfield background visible
- Accretion disk glowing around the black hole
- ImGui control panel on left side

### Controls:
- **Left Mouse + Drag**: Rotate camera
- **Mouse Wheel**: Zoom in/out
- **Right Mouse + Drag**: Pan camera

### Performance (RTX 5060 @ 1080p):
- **120+ FPS** typical

---

## 🎨 Things to Try

### In the UI Panel:

1. **Black Hole → Spin**: Drag to 0.95
   - See frame dragging effects
   - Watch accretion disk behavior change

2. **Accretion Disk → Temperature**: Adjust color

3. **Presets → Near-Maximal Kerr**: Instant extreme black hole

4. **Rendering → Exposure**: Adjust brightness

### Camera:
- Zoom very close to see strong lensing
- Orbit around to see Einstein ring
- Look edge-on at accretion disk

---

## 🐛 Troubleshooting

### "CMake Error" or "Cannot find X"
→ Run `.\download_deps.ps1` again
→ Ensure GLAD is manually set up (see Step 3)

### Black Screen
→ Update GPU drivers
→ Ensure RTX/OpenGL 4.6 support

### Crashes on Startup
→ Run from project root: `.\build\bin\Release\BlackholeSim.exe`
→ Check `shaders/` folder exists

### Low FPS
→ Lower quality in "Rendering" panel
→ Disable bloom
→ Reduce window size

---

## 📚 Learn More

- **README.md**: Full documentation
- **SETUP_INSTRUCTIONS.md**: Detailed setup guide
- **external/README.md**: Dependency details

---

## 🎯 Quick Test Checklist

After building, verify:
- [ ] Window opens without errors
- [ ] Black hole visible in center
- [ ] Stars in background
- [ ] Accretion disk glowing
- [ ] UI panel responds
- [ ] Camera rotates with mouse
- [ ] FPS counter shows > 60

**All checked? You're ready to explore black holes!** 🌌
