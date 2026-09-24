# Tasks: Navigation Profiling and Evaluation

**Input**: Design documents from `/specs/001-navigation-profiling/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`

**Tests**: Required by the feature specification and constitution. For each behavior, add the test first, confirm it fails, then implement.

**Organization**: Tasks are grouped by user story so each increment can be demonstrated and validated independently.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Safe to execute in parallel because files and prerequisites do not conflict.
- **[USn]**: Maps the task to a user story in `spec.md`.

## Phase 1: Setup

- [X] T001 Preserve relevant lessons from the legacy package in `docs/legacy-baseline.md`, then remove `ros2_ws/src/mobile_manipulator_demo`
- [X] T002 Create package skeletons and manifests for `ros2_ws/src/robot_sim_bringup`, `ros2_ws/src/navigation_evaluation`, `ros2_ws/src/sensor_fault_injection`, and `ros2_ws/src/diagnostics_ui`
- [X] T003 [P] Add C++20 warning, formatting, and lint settings to each package `CMakeLists.txt` and repository validation scripts
- [X] T004 Record exact simulator, bridge, QoS, robot asset, GPU driver, ROS compatibility, and rosbag2-optional evidence findings in `specs/001-navigation-profiling/research.md`
- [X] T005 Update `docker/Dockerfile` with the dependencies validated by T004 and verify after T001 that no legacy package paths remain

## Phase 2: Foundational (Blocking Prerequisites)

- [X] T006 [P] Add versioned YAML fixture and naming-rule tests in `ros2_ws/src/navigation_evaluation/config/scenarios` and `ros2_ws/src/navigation_evaluation/test/test_scenario_schema.cpp`
- [ ] T007 [P] Add artifact fixtures and failing parser/writer tests for atomic records, partial JSONL, hashing, and every required event category in `ros2_ws/src/navigation_evaluation/test/test_artifact_store.cpp`
- [ ] T008 [P] Add metric clock-domain and aggregation tests in `ros2_ws/src/navigation_evaluation/test/test_metrics.cpp`
- [X] T009 Implement scenario model and validation in `ros2_ws/src/navigation_evaluation/include/navigation_evaluation/scenario.hpp` and `ros2_ws/src/navigation_evaluation/src/scenario.cpp`
- [X] T010 Implement partial-safe artifact storage in `ros2_ws/src/navigation_evaluation/include/navigation_evaluation/artifact_store.hpp` and `ros2_ws/src/navigation_evaluation/src/artifact_store.cpp`
- [ ] T011 Implement typed metric and aggregate logic in `ros2_ws/src/navigation_evaluation/include/navigation_evaluation/metrics.hpp` and `ros2_ws/src/navigation_evaluation/src/metrics.cpp`
- [ ] T012 [P] Create the robot URDF/Xacro, sensor configuration, and frame diagram in `ros2_ws/src/robot_sim_bringup/urdf`, `ros2_ws/src/robot_sim_bringup/config`, and `docs/frames.md`
- [ ] T013 [P] Create the Isaac Sim world and versioned asset manifest in `ros2_ws/src/robot_sim_bringup/worlds` and `ros2_ws/src/robot_sim_bringup/config/assets.yaml`
- [ ] T014 Add failing launch tests for required nodes, topics, QoS, sensor stamps, and connected TF in `ros2_ws/src/robot_sim_bringup/test/test_interface_contract.py`
- [ ] T015 Implement simulator, localization, and Nav2 bringup in `ros2_ws/src/robot_sim_bringup/launch/evaluation.launch.py`

**Checkpoint**: Simulator, contracts, scenario parsing, and artifact primitives are testable before user-story implementation.

## Phase 3: User Story 1 - Execute and Measure a Navigation Task (Priority: P1) MVP

**Goal**: Run healthy single and batch navigation tasks with trustworthy artifacts.

**Independent Test**: Execute the healthy quickstart and confirm one complete run plus an exact 20-trial summary.

- [ ] T016 [P] [US1] Add failing lifecycle, cancellation, and terminal-classification tests covering readiness, navigation, stale-sensor, timeout, infrastructure, and internal failures in `ros2_ws/src/navigation_evaluation/test/test_scenario_runner.cpp`
- [ ] T017 [P] [US1] Add failing end-to-end healthy scenario and trial-reset isolation tests, including unique run IDs and empty initial metrics, in `ros2_ws/src/navigation_evaluation/test/test_healthy_navigation.py`
- [ ] T018 [US1] Implement the lifecycle-aware Nav2 scenario runner and all six machine-readable failure classifications in `ros2_ws/src/navigation_evaluation/src/scenario_runner_node.cpp`
- [ ] T019 [US1] Implement lifecycle, navigation, recovery, terminal, and paired `/scan_raw` versus `/scan` timing events in `ros2_ws/src/navigation_evaluation/src/metrics_collector_node.cpp`
- [ ] T020 [US1] Implement batch orchestration and reset checks in `ros2_ws/src/navigation_evaluation/src/run_batch.cpp`
- [ ] T021 [US1] Add `healthy.yaml` and wire it into `ros2_ws/src/robot_sim_bringup/launch/evaluation.launch.py`
- [ ] T022 [US1] Run the healthy quickstart, archive a sample fixture under `ros2_ws/src/navigation_evaluation/test/fixtures`, and document observed baseline limitations in `docs/reproducibility.md`

**Checkpoint**: User Story 1 is independently demoable and is the MVP.

## Phase 4: User Story 2 - Observe Failure and Restart Behavior (Priority: P2)

**Goal**: Inject visible lidar faults, terminate safely, and prove clean restart behavior.

**Independent Test**: Run delay and outage scenarios, then a healthy scenario in the restarted system; inspect fault events and run IDs.

- [ ] T023 [P] [US2] Add failing delay queue, drop window, ordering, and clock-jump tests in `ros2_ws/src/sensor_fault_injection/test/test_fault_policy.cpp`
- [ ] T024 [P] [US2] Add failing fault event contract tests in `ros2_ws/src/sensor_fault_injection/test/test_fault_events.cpp`
- [ ] T025 [US2] Implement scenario-driven lidar fault policies in `ros2_ws/src/sensor_fault_injection/src/fault_policy.cpp`
- [ ] T026 [US2] Implement the raw-to-impaired lidar relay and fault event publisher in `ros2_ws/src/sensor_fault_injection/src/lidar_fault_injector_node.cpp`
- [ ] T027 [US2] Route Nav2 through the impaired lidar topic in `ros2_ws/src/robot_sim_bringup/config/nav2_params.yaml`
- [ ] T028 [P] [US2] Add delay and outage scenarios in `ros2_ws/src/navigation_evaluation/config/scenarios/lidar_delay.yaml` and `ros2_ws/src/navigation_evaluation/config/scenarios/lidar_outage.yaml`
- [ ] T029 [US2] Add failing timeout, process interruption, clean-restart, and configured-deadline-plus-2-second shutdown tests in `ros2_ws/src/navigation_evaluation/test/test_fault_and_restart.py`
- [ ] T030 [US2] Implement cleanup within the configured deadline plus 2 seconds and interrupted-run recovery in `ros2_ws/src/navigation_evaluation/src/scenario_runner_node.cpp` and `ros2_ws/src/navigation_evaluation/src/artifact_store.cpp`

**Checkpoint**: User Stories 1 and 2 independently pass their scenario tests.

## Phase 5: User Story 3 - Diagnose and Compare Runs (Priority: P3)

**Goal**: Provide responsive live diagnostics and honest run comparison in Qt.

**Independent Test**: Observe a live scenario and compare healthy and faulted 20-trial batches, including partial-data handling.

- [ ] T031 [P] [US3] Add failing artifact compatibility and comparison-model tests in `ros2_ws/src/diagnostics_ui/test/test_run_comparison.cpp`
- [ ] T032 [P] [US3] Add failing ROS worker, raw/impaired freshness, stale-data, thread-affinity, and 20 Hz load tests with a 10 ms p95 callback limit in `ros2_ws/src/diagnostics_ui/test/test_ros_worker.cpp`
- [ ] T033 [US3] Implement the non-GUI artifact reader and comparison model in `ros2_ws/src/diagnostics_ui/src/run_comparison.cpp`
- [ ] T034 [US3] Implement the off-GUI-thread ROS executor worker and immutable UI snapshots, including raw and impaired lidar freshness, in `ros2_ws/src/diagnostics_ui/src/ros_worker.cpp`
- [ ] T035 [US3] Implement live status, sensor freshness, fault, and event views in `ros2_ws/src/diagnostics_ui/src/main_window.cpp`
- [ ] T036 [US3] Implement run-set selection and comparison tables/plots in `ros2_ws/src/diagnostics_ui/src/main_window.cpp`
- [ ] T037 [US3] Configure headless QTest/CTest execution and add interaction and incomplete-artifact tests in `ros2_ws/src/diagnostics_ui/test/test_main_window.cpp`

**Checkpoint**: All three user stories are independently demonstrable.

## Phase 6: Evidence and Cross-Cutting Quality

- [ ] T038 [P] Add architecture and detailed data-flow diagrams to `docs/architecture.md`
- [ ] T039 [P] Add a debugging write-up based on one captured timing or TF failure to `docs/debugging.md`
- [ ] T040 [P] Extend `scripts/run_validation.ps1` and add `scripts/run_validation.sh` for unit, contract, launch, restart, and schema checks
- [ ] T041 Execute at least 20 trials for each published scenario and add the generated comparison table to `docs/results.md`
- [ ] T042 Validate `specs/001-navigation-profiling/quickstart.md` on a clean supported host and replace provisional commands or assumptions with observed facts
- [ ] T043 Audit every result in `docs/results.md` for at least 20 trials or an explicit exploratory label, then run Spec Kit analysis and convergence and complete any appended tasks

## Dependencies & Execution Order

- Setup (T001-T005) precedes foundational work.
- Foundational work (T006-T015) blocks all user stories.
- User Story 1 is the MVP and provides run data needed by User Stories 2 and 3.
- User Story 2 depends on the runner and metrics contracts from User Story 1.
- User Story 3 depends on stable artifacts from User Story 1; its ROS worker can proceed in parallel with User Story 2.
- Evidence work starts when the relevant story is stable; final benchmark and convergence tasks follow all selected stories.

## Implementation Strategy

Complete and validate User Story 1 before expanding scope. Preserve raw simulator streams and artifact compatibility as User Story 2 lands. Build the UI against stored fixtures first, then attach live ROS data. At every checkpoint, run focused tests first and the repository-wide validation command second.