param(
    [string]$Workspace = "ros2_ws"
)

$ErrorActionPreference = "Stop"

python -m unittest discover "$Workspace\src\mobile_manipulator_demo\test"

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

