# Simple automated dependency download (non-interactive)
Write-Host "Downloading dependencies..." -ForegroundColor Cyan

# Create external directory
New-Item -ItemType Directory -Force -Path "external" | Out-Null

# GLFW
if (-not (Test-Path "external/glfw")) {
    Write-Host "Downloading GLFW..." -ForegroundColor Yellow
    git clone --depth 1 --branch 3.3.9 https://github.com/glfw/glfw.git external/glfw 2>$null
    if ($?) { Write-Host "  OK" -ForegroundColor Green } else { Write-Host "  FAILED" -ForegroundColor Red }
}

# GLM
if (-not (Test-Path "external/glm")) {
    Write-Host "Downloading GLM..." -ForegroundColor Yellow
    git clone --depth 1 https://github.com/g-truc/glm.git external/glm 2>$null
    if ($?) { Write-Host "  OK" -ForegroundColor Green } else { Write-Host "  FAILED" -ForegroundColor Red }
}

# ImGui
if (-not (Test-Path "external/imgui")) {
    Write-Host "Downloading ImGui..." -ForegroundColor Yellow
    git clone --depth 1 --branch v1.90.1 https://github.com/ocornut/imgui.git external/imgui 2>$null
    if ($?) { Write-Host "  OK" -ForegroundColor Green } else { Write-Host "  FAILED" -ForegroundColor Red }
}

# stb_image
New-Item -ItemType Directory -Force -Path "external/stb" | Out-Null
if (-not (Test-Path "external/stb/stb_image.h")) {
    Write-Host "Downloading stb_image..." -ForegroundColor Yellow
    try {
        Invoke-WebRequest -Uri "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" -OutFile "external/stb/stb_image.h" -ErrorAction Stop
        Write-Host "  OK" -ForegroundColor Green
    } catch {
        Write-Host "  FAILED" -ForegroundColor Red
    }
}

Write-Host "`nDependencies downloaded!" -ForegroundColor Green
Write-Host "NOTE: GLAD must be manually generated from https://glad.dav1d.de/" -ForegroundColor Yellow
