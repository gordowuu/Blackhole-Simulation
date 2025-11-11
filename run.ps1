# Black Hole Simulation - Launch Script

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Black Hole Simulation - Launch" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

# Check if executable exists
$exePath = "build\bin\BlackholeSim.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "ERROR: Executable not found!" -ForegroundColor Red
    Write-Host "Please run build.ps1 first to compile the project." -ForegroundColor Yellow
    exit 1
}

Write-Host "Launching Black Hole Simulation..." -ForegroundColor Green
Write-Host "Window title: Black Hole Simulation - RTX Accelerated`n" -ForegroundColor Gray

# Navigate to bin directory and run
Set-Location "build\bin"
.\BlackholeSim.exe

# Return to root
Set-Location ..\..
