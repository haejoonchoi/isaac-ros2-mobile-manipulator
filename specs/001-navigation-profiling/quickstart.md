# Quickstart Validation: Navigation Profiling and Evaluation

This is the target clean-host validation journey. Commands become executable as their corresponding tasks land; until then, the README labels the end-to-end setup as planned.

## Prerequisites

- Ubuntu 24.04 with a supported NVIDIA driver and GPU.
- Pinned NVIDIA Isaac Sim release documented by Phase 0 research.
- ROS 2 Jazzy and the repository development container or equivalent dependencies.
- Qt 6 development and runtime packages.

## Build and Test

```bash
cd ros2_ws
colcon build --symlink-install
source install/setup.bash
colcon test
colcon test-result --verbose
```

## Healthy Run

```bash
ros2 launch robot_sim_bringup evaluation.launch.py scenario:=healthy.yaml
```

Verify that the robot reaches the goal, the run is terminal and complete, and `artifacts/<batch-id>/runs/<run-id>/` contains `run.json` and `events.jsonl`.

## Batch Evaluation

```bash
ros2 run navigation_evaluation run_batch --scenario healthy.yaml --trials 20
```

Verify that the summary contains exactly 20 trials, success rate, duration statistics, environment versions, and configuration identity.

## Fault and Restart

```bash
ros2 run navigation_evaluation run_batch --scenario lidar_outage.yaml --trials 20
ros2 lifecycle set /scenario_runner shutdown
ros2 launch robot_sim_bringup evaluation.launch.py scenario:=healthy.yaml
```

Verify that every scheduled outage has requested and observed events, all tasks terminate within their deadlines, and the restarted healthy run uses a new run ID with no stale events.

## Diagnostics

```bash
ros2 run diagnostics_ui diagnostics_ui
```

Verify live state updates at least twice per second. Load the healthy and faulted batch directories and confirm the comparison includes success rate, median duration, sensor timing, and failure classes.

## Reproducibility Record

Archive the batch metadata, summary, scenario files, package revision, simulator version, ROS distribution, GPU/driver details, and host-load notes. Repeat runs may vary; compare distributions and disclose the observed spread.