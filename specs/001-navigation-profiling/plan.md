# Implementation Plan: Navigation Profiling and Evaluation (Superseded)

**Branch**: `001-navigation-profiling` | **Date**: 2026-09-21 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/001-navigation-profiling/spec.md`

> This plan is retained for historical reference only. It is superseded by
> `specs/002-gazebo-integration/` and must not be used to implement the
> project. The active simulator direction is Gazebo.

## Summary

Replace the legacy Python task scaffold incrementally with a C++20 ROS 2 system that drives Nav2 goals in Isaac Sim, isolates controlled sensor faults through relay topics, records versioned run evidence, and presents live and historical diagnostics in Qt 6. Deliver the healthy navigation runner first, then fault/restart behavior, then the UI.

## Technical Context

**Language/Version**: C++20 for production nodes and Qt UI; Python 3 for launch and test glue

**Primary Dependencies**: ROS 2 Jazzy, `rclcpp`, `rclcpp_lifecycle`, Nav2 interfaces, `tf2_ros`, rosbag2 where useful, Qt 6, NVIDIA Isaac Sim ROS 2 bridge

**Storage**: Versioned JSONL per-run events, YAML scenario definitions, CSV batch summaries, optional rosbag2 recordings

**Testing**: `ament_cmake_gtest`, `launch_testing`, Nav2 system tests, schema fixtures, headless Qt tests

**Target Platform**: Ubuntu 24.04 workstation with a compatible NVIDIA GPU; Windows remains a documentation/editing host only

**Project Type**: ROS 2 workspace plus desktop diagnostic application

**Performance Goals**: UI refresh at least 2 Hz; diagnostic worker p95 callback duration at most 10 ms under 20 Hz synthetic input; bounded task completion

**Constraints**: One simulator; no mixed-clock arithmetic; faults must preserve raw streams; all waits bounded; 20 trials per published configuration

**Scale/Scope**: One robot, one map, three initial scenarios (healthy, lidar delay, lidar outage), local execution, tens to hundreds of runs

## Constitution Check

*GATE: Must pass before Phase 0 research and be re-checked after Phase 1 design.*

| Principle | Design evidence | Status |
| --- | --- | --- |
| Measurable Behavior First | Spec defines terminal outcomes, metrics, units, and trial counts | PASS |
| ROS Contracts Are Explicit | `contracts/ros-interfaces.md` defines topics, actions, QoS, frames, and clocks | PASS |
| Test First | Tasks place failing tests before each implementation slice | PASS |
| Faults Are Controlled and Visible | Relay boundary preserves raw lidar and emits requested/observed fault events | PASS |
| Reproducible, Not Perfectly Deterministic | Artifacts include versions/config identity; reports require limitations | PASS |

Post-design re-check: PASS. The data model includes clock domains and configuration identity; contracts distinguish raw and impaired topics; no exception is required.

## Project Structure

### Documentation (this feature)

```text
specs/001-navigation-profiling/
|-- spec.md
|-- plan.md
|-- research.md
|-- data-model.md
|-- quickstart.md
|-- contracts/
|   |-- artifact-schema.md
|   `-- ros-interfaces.md
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

artifacts/                       # Generated and gitignored except fixtures
docker/
docs/
scripts/
```

**Structure Decision**: Four focused ROS 2 packages keep simulator assets, evaluation ownership, fault relay behavior, and GUI dependencies independently testable. Contracts use standard ROS 2 interfaces where possible; project messages are introduced only for run and fault events that lack a standard equivalent.

## Delivery Design

### Phase 0: Resolve Compatibility and Measurement Semantics

- Validate one exact Isaac Sim release with ROS 2 Jazzy and pin the container/image and robot asset revision.
- Measure bridge timestamp behavior and define ROS-time versus steady-time usage.
- Verify Nav2 reset and lifecycle behavior after simulator reset.
- Confirm Qt 6 ROS executor integration without running robot callbacks on the GUI thread.

### Phase 1: Healthy Navigation Vertical Slice

- Bring up robot, sensors, TF, localization, and Nav2 in one command.
- Add a lifecycle-aware C++ scenario runner with readiness checks and bounded goal execution.
- Add a metrics collector that commits partial-safe JSONL events and terminal run records.
- Validate one run and a 20-trial batch before adding fault machinery.

### Phase 2: Fault and Restart Slice

- Route raw lidar through a relay that applies scenario-defined delay or outage schedules.
- Keep source and impaired topics separate and record requested versus observed fault timing.
- Add cancellation, cleanup, restart, interrupted-run recovery, and stale-state tests.

### Phase 3: Diagnostic Interface

- Subscribe to live run/fault/navigation state through a ROS worker thread.
- Display state, sensor freshness, active faults, and recent events.
- Parse versioned artifacts and compare healthy and faulted run sets.

### Phase 4: Evidence Package

- Run clean-host quickstart and benchmark matrices.
- Publish architecture and TF diagrams, benchmark tables, demo media, and a debugging narrative based on captured logs and metrics.
- Run Spec Kit analysis and convergence until no acceptance gap remains.

## Complexity Tracking

No constitution violations are currently required.