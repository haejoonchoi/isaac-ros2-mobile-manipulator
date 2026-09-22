param(
    [string]$Workspace = "ros2_ws"
)

$ErrorActionPreference = "Stop"

$PackagePath = Join-Path $Workspace "src\mobile_manipulator_demo"
Push-Location $PackagePath
try {
    python -m unittest discover test
}
finally {
    Pop-Location
}

if (Get-Command colcon -ErrorAction SilentlyContinue) {
    Push-Location $Workspace
    try {
        colcon test --packages-select mobile_manipulator_demo
        colcon test-result --verbose
    }
    finally {
        Pop-Location
    }
}
else {
    Write-Host "colcon not found; skipped ROS2 package tests."
}

