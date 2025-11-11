# Black Hole Simulation - Build Script

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Black Hole Simulation - Build Script" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

# Check if dependencies are set up
$requiredDeps = @(
    "external/glfw/CMakeLists.txt",
    "external/glm/glm/glm.hpp",
    "external/imgui/imgui.h",
    "external/stb/stb_image.h",
    "external/glad/include/glad/glad.h"
)

$missingDeps = @()
foreach ($dep in $requiredDeps) {
    if (-not (Test-Path $dep)) {
        $missingDeps += $dep
    }
}

if ($missingDeps.Count -gt 0) {
    Write-Host "ERROR: Missing dependencies!" -ForegroundColor Red
    Write-Host "Please run setup.ps1 first to install dependencies.`n" -ForegroundColor Yellow
    Write-Host "Missing:" -ForegroundColor Yellow
    foreach ($dep in $missingDeps) {
        Write-Host "  - $dep" -ForegroundColor Red
    }
    exit 1
}

Write-Host "✓ All dependencies found`n" -ForegroundColor Green

# Ask for build type
Write-Host "Select build configuration:" -ForegroundColor Yellow
Write-Host "  1. Debug" -ForegroundColor White
Write-Host "  2. Release (Recommended)" -ForegroundColor White
Write-Host "  3. RelWithDebInfo" -ForegroundColor White

$buildType = Read-Host "`nEnter choice (1-3)"

switch ($buildType) {
    "1" { $config = "Debug" }
    "2" { $config = "Release" }
    "3" { $config = "RelWithDebInfo" }
    default { $config = "Release"; Write-Host "Invalid choice, defaulting to Release" -ForegroundColor Yellow }
}

Write-Host "`nBuilding in $config mode...`n" -ForegroundColor Green

# Create build directory
if (Test-Path "build") {
    Write-Host "Build directory exists. Clean build? (Y/N)" -ForegroundColor Yellow
    $clean = Read-Host
    if ($clean -eq "Y" -or $clean -eq "y") {
        Remove-Item -Recurse -Force "build"
        Write-Host "Cleaned build directory`n" -ForegroundColor Green
    }
}

New-Item -ItemType Directory -Force -Path "build" | Out-Null
Set-Location "build"

# Generate project files
Write-Host "Generating Visual Studio solution..." -ForegroundColor Yellow
cmake .. -G "Visual Studio 17 2022" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "`nERROR: CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host "✓ Project files generated`n" -ForegroundColor Green

# Build
Write-Host "Building project (this may take a few minutes)..." -ForegroundColor Yellow
cmake --build . --config $config -- /maxcpucount

if ($LASTEXITCODE -ne 0) {
    Write-Host "`nERROR: Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host "`n✓ Build completed successfully!" -ForegroundColor Green

# Return to root
Set-Location ..

# Show results
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "Build Complete!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$exePath = "build\bin\$config\BlackholeSim.exe"
if (Test-Path $exePath) {
    Write-Host "Executable: $exePath" -ForegroundColor Green
    Write-Host "`nRun the simulation:" -ForegroundColor Yellow
    Write-Host "  cd build\bin\$config" -ForegroundColor White
    Write-Host "  .\BlackholeSim.exe" -ForegroundColor White
    
    Write-Host "`nOr run from here:" -ForegroundColor Yellow
    Write-Host "  .\$exePath" -ForegroundColor White
    
    Write-Host "`nLaunch now? (Y/N)" -ForegroundColor Yellow
    $launch = Read-Host
    if ($launch -eq "Y" -or $launch -eq "y") {
        Write-Host "`nLaunching Black Hole Simulation..." -ForegroundColor Cyan
        Start-Process $exePath
    }
} else {
    Write-Host "WARNING: Executable not found at expected location" -ForegroundColor Yellow
}

Write-Host "`nPress Enter to exit..." -ForegroundColor Gray
Read-Host
