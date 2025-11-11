# Black Hole Simulation - Dependency Setup Script
# Run this script from the project root directory

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Black Hole Simulation - Setup Script" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

# Check for Git
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: Git is not installed or not in PATH" -ForegroundColor Red
    Write-Host "Please install Git from https://git-scm.com/" -ForegroundColor Yellow
    exit 1
}

# Check for CMake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: CMake is not installed or not in PATH" -ForegroundColor Red
    Write-Host "Please install CMake from https://cmake.org/" -ForegroundColor Yellow
    exit 1
}

Write-Host "✓ Git and CMake found`n" -ForegroundColor Green

# Create external directory
Write-Host "Creating external directory..." -ForegroundColor Yellow
New-Item -ItemType Directory -Force -Path "external" | Out-Null

# GLFW
Write-Host "`nSetting up GLFW..." -ForegroundColor Yellow
if (Test-Path "external/glfw") {
    Write-Host "  GLFW already exists, skipping..." -ForegroundColor Gray
} else {
    git clone --depth 1 --branch 3.3.9 https://github.com/glfw/glfw.git external/glfw
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  ✓ GLFW downloaded" -ForegroundColor Green
    } else {
        Write-Host "  ✗ GLFW download failed" -ForegroundColor Red
    }
}

# GLM
Write-Host "`nSetting up GLM..." -ForegroundColor Yellow
if (Test-Path "external/glm") {
    Write-Host "  GLM already exists, skipping..." -ForegroundColor Gray
} else {
    git clone --depth 1 https://github.com/g-truc/glm.git external/glm
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  ✓ GLM downloaded" -ForegroundColor Green
    } else {
        Write-Host "  ✗ GLM download failed" -ForegroundColor Red
    }
}

# ImGui
Write-Host "`nSetting up ImGui..." -ForegroundColor Yellow
if (Test-Path "external/imgui") {
    Write-Host "  ImGui already exists, skipping..." -ForegroundColor Gray
} else {
    git clone --depth 1 --branch v1.90.1 https://github.com/ocornut/imgui.git external/imgui
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  ✓ ImGui downloaded" -ForegroundColor Green
    } else {
        Write-Host "  ✗ ImGui download failed" -ForegroundColor Red
    }
}

# stb_image
Write-Host "`nSetting up stb_image..." -ForegroundColor Yellow
New-Item -ItemType Directory -Force -Path "external/stb" | Out-Null
if (Test-Path "external/stb/stb_image.h") {
    Write-Host "  stb_image.h already exists, skipping..." -ForegroundColor Gray
} else {
    try {
        Invoke-WebRequest -Uri "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" -OutFile "external/stb/stb_image.h"
        Write-Host "  ✓ stb_image.h downloaded" -ForegroundColor Green
    } catch {
        Write-Host "  ✗ stb_image.h download failed" -ForegroundColor Red
    }
}

# GLAD - Special handling (needs to be generated)
Write-Host "`nSetting up GLAD..." -ForegroundColor Yellow
if (Test-Path "external/glad") {
    Write-Host "  GLAD already exists, skipping..." -ForegroundColor Gray
} else {
    Write-Host "  GLAD requires manual setup:" -ForegroundColor Yellow
    Write-Host "    1. Visit https://glad.dav1d.de/" -ForegroundColor Cyan
    Write-Host "    2. Select:" -ForegroundColor Cyan
    Write-Host "       - Language: C/C++" -ForegroundColor White
    Write-Host "       - Specification: OpenGL" -ForegroundColor White
    Write-Host "       - gl Version: 4.6" -ForegroundColor White
    Write-Host "       - Profile: Core" -ForegroundColor White
    Write-Host "       - Check 'Generate a loader'" -ForegroundColor White
    Write-Host "    3. Click GENERATE" -ForegroundColor Cyan
    Write-Host "    4. Download and extract to external/glad/" -ForegroundColor Cyan
    Write-Host "  " -NoNewline
    Write-Host "Press Enter after you have set up GLAD..." -ForegroundColor Yellow
    Read-Host
}

# Verify GLAD structure
if (Test-Path "external/glad/include/glad/glad.h") {
    Write-Host "  ✓ GLAD found and verified" -ForegroundColor Green
} else {
    Write-Host "  ✗ GLAD not properly set up - please complete manual setup" -ForegroundColor Red
    Write-Host "    Expected structure:" -ForegroundColor Yellow
    Write-Host "      external/glad/include/glad/glad.h" -ForegroundColor Gray
    Write-Host "      external/glad/include/KHR/khrplatform.h" -ForegroundColor Gray
    Write-Host "      external/glad/src/glad.c" -ForegroundColor Gray
}

# Summary
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "Setup Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$dependencies = @(
    @{Name="GLFW"; Path="external/glfw/CMakeLists.txt"},
    @{Name="GLM"; Path="external/glm/glm/glm.hpp"},
    @{Name="ImGui"; Path="external/imgui/imgui.h"},
    @{Name="stb_image"; Path="external/stb/stb_image.h"},
    @{Name="GLAD"; Path="external/glad/include/glad/glad.h"}
)

$allFound = $true
foreach ($dep in $dependencies) {
    if (Test-Path $dep.Path) {
        Write-Host "✓ $($dep.Name)" -ForegroundColor Green
    } else {
        Write-Host "✗ $($dep.Name) - NOT FOUND" -ForegroundColor Red
        $allFound = $false
    }
}

if ($allFound) {
    Write-Host "`nAll dependencies ready!" -ForegroundColor Green
    Write-Host "You can now build the project:" -ForegroundColor Cyan
    Write-Host "  mkdir build" -ForegroundColor White
    Write-Host "  cd build" -ForegroundColor White
    Write-Host "  cmake .. -G `"Visual Studio 17 2022`" -A x64" -ForegroundColor White
    Write-Host "  cmake --build . --config Release" -ForegroundColor White
} else {
    Write-Host "`nSome dependencies are missing. Please complete setup." -ForegroundColor Yellow
    Write-Host "See external/README.md for detailed instructions." -ForegroundColor Cyan
}

Write-Host "`nPress Enter to exit..." -ForegroundColor Gray
Read-Host
