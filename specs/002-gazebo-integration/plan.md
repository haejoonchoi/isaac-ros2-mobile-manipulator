# Implementation Plan: Gazebo Integration for Constrained Hardware

**Branch**: `002-gazebo-integration` | **Date**: 2026-09-23 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/002-gazebo-integration/spec.md`

## Summary

Replace the superseded Isaac Sim runtime with Gazebo Harmonic on Ubuntu 24.04
and ROS 2 Jazzy. Use the standard `ros_gz` bridge for simulation clock,
odometry, lidar, and entity services; keep the evaluation runner, fault relay,
artifact store, and Qt diagnostics simulator-neutral. Deliver a server-only
headless profile first, then add graphical execution and the navigation,
fault/restart, and comparison workflows.

## Technical Context

**Language/Version**: C++20 for production ROS 2 nodes and Qt UI; Python 3 for
launch and test glue

**Primary Dependencies**: ROS 2 Jazzy, Gazebo Harmonic, `ros_gz_sim`,
`ros_gz_bridge`, `ros_gz_interfaces`, `rclcpp`, Nav2, `tf2_ros`, Qt 6,
`ament_cmake_gtest`, `launch_testing`

**Storage**: Versioned JSONL per-run events, YAML scenario and execution
profiles, CSV batch summaries, optional rosbag2 recordings

**Testing**: `ament_cmake_gtest`, `launch_testing`, scenario-level integration
tests, headless QTest/CTest, and constrained-hardware smoke tests

**Target Platform**: Ubuntu 24.04 workstation with ROS 2 Jazzy and Gazebo
Harmonic; graphical mode is optional, server-only headless mode is required

**Project Type**: ROS 2 workspace plus desktop diagnostic application

**Performance Goals**: Healthy simulation readiness within 120 seconds;
positive simulation-clock progress within 20 seconds; UI refresh at least 2 Hz;
diagnostic worker p95 callback duration at most 10 ms under 20 Hz synthetic
input

**Constraints**: Four physical CPU cores and 16 GB RAM are the reference
minimum; no high-end GPU is required; physics and navigation must remain
usable in server-only mode; all waits are bounded; no mixed-clock arithmetic;
20 trials per published configuration

**Scale/Scope**: One differential-drive robot, one low-complexity world, three
initial scenarios (healthy, lidar delay, lidar outage), local execution, tens
to hundreds of runs

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

| Principle | Design evidence | Status |
|---|---|---|
| Measurable Behavior First | The spec defines terminal outcomes, clock-aware metrics, resource observations, and trial thresholds. | PASS |
| ROS Contracts Are Explicit | `contracts/ros-interfaces.md` defines topics, services, QoS, frames, timestamps, and `/clock`. | PASS |
| Test First | Research and design identify unit, contract, launch, restart, scenario, and resource smoke tests before implementation. | PASS |
| Faults Are Controlled and Visible | The relay preserves raw lidar and records requested versus observed fault timing. | PASS |
| Reproducible, Not Perfectly Deterministic | Profiles record versions, scenario hashes, execution mode, host assumptions, and known variation. | PASS |
| Gazebo Scope | Gazebo Harmonic is the sole active simulator; Isaac Sim is superseded. | PASS |

Post-design re-check: PASS. The design uses the standard Gazebo/ROS boundary,
keeps clock domains explicit, and includes bounded reset and resource-failure
paths.

## Project Structure

### Documentation (this feature)

```text
specs/002-gazebo-integration/
|-- spec.md
|-- plan.md
|-- research.md
|-- data-model.md
|-- quickstart.md
|-- contracts/
|   |-- ros-interfaces.md
|   `-- artifact-schema.md
|-- checklists/
|   `-- requirements.md
`-- tasks.md
```

### Source Code (repository root)

```text
ros2_ws/src/
|-- robot_sim_bringup/
|   |-- config/
|   |-- launch/
|   |-- maps/
|   |-- models/
|   |-- urdf/
|   `-- worlds/
|-- navigation_evaluation/
|   |-- config/scenarios/
|   |-- include/navigation_evaluation/
|   |-- src/
|   `-- test/
|-- sensor_fault_injection/
|   |-- include/sensor_fault_injection/
|   |-- src/
|   `-- test/
`-- diagnostics_ui/
    |-- include/diagnostics_ui/
    |-- src/
    `-- test/

docker/
docs/
scripts/
```

**Structure Decision**: Keep the four existing package boundaries. Gazebo world,
models, bridge configuration, and execution profiles belong to
`robot_sim_bringup`; navigation evaluation, fault injection, and Qt diagnostics
remain independently testable and consume documented ROS/artifact contracts.

## Delivery Design

### Phase 0: Gazebo Compatibility and Resource Baseline

- Pin Gazebo Harmonic as the ROS 2 Jazzy pairing and record package/version
  evidence.
- Validate `ros_gz_sim` launch/spawn, `ros_gz_bridge`, `/clock`, lidar,
  odometry, TF, and Nav2 readiness in a minimal world.
- Establish server-only and optional graphical execution profiles with explicit
  CPU, memory, clock-progress, and readiness thresholds.
- Validate delete-reset-respawn behavior and document known clock-reset
  boundaries.

### Phase 1: Healthy Navigation Vertical Slice

- Add the robot model, low-complexity world, bridge configuration, TF contracts,
  localization, and Nav2 bringup.
- Implement readiness checks for clock progress, transforms, sensors, services,
  and resource profile.
- Reuse the scenario runner, metrics, artifact store, and batch orchestration
  with Gazebo execution metadata.
- Validate one healthy run and an exact 20-trial batch in server-only mode.

### Phase 2: Fault and Restart Slice

- Route raw bridged lidar through a relay to the impaired navigation topic.
- Add bounded delay/drop schedules and requested-versus-observed fault events.
- Reset through explicit delete, world reset, respawn, localization reset, and
  costmap clearing; verify one robot entity and a fresh run ID.
- Classify simulation, resource, task, injected-fault response, and
  infrastructure failures.

### Phase 3: Diagnostic Interface

- Subscribe through a ROS worker thread and expose immutable snapshots to Qt.
- Display live navigation state, raw/impaired sensor freshness, faults, events,
  resource profile, and clock status.
- Parse versioned artifacts and compare healthy and faulted run sets, including
  partial and incompatible data.

### Phase 4: Evidence Package

- Run the clean-host quickstart and constrained-hardware smoke matrix.
- Publish architecture/TF diagrams, resource-budget evidence, benchmark tables,
  and a debugging narrative based on captured timing or reset evidence.
- Run Spec Kit analysis and convergence before declaring the feature complete.

## Complexity Tracking

No constitution violations are currently required.
