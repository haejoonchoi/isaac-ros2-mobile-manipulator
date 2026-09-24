# Delivery milestones

The feature-level source of truth is `specs/002-gazebo-integration/`. The 001 navigation-profiling artifacts are historical background. These milestones summarize the review gates.

## M1: Navigation baseline

- Pin the supported ROS 2, Gazebo Harmonic, Nav2, and robot asset versions.
- Spawn a differential-drive robot with lidar, odometry, IMU, simulation clock, and a documented TF tree.
- Send one navigation goal through a C++ scenario runner and record its terminal result.
- Gate: a fresh environment can execute the healthy scenario from documented commands.

## M2: Repeatable evaluation

- Define scenario configuration and versioned run-artifact schemas.
- Reset the world and robot between trials without stale state leaking into the next run.
- Record success, failure class, task duration, sensor age, inter-arrival time, and recovery events.
- Gate: a batch produces complete machine-readable artifacts and an aggregate report.

## M3: Controlled failures and recovery

- Add deterministic fault schedules for delayed and missing lidar messages.
- Expose active faults and distinguish injected faults from infrastructure failures.
- Implement bounded timeout, cancellation, cleanup, and restart behavior.
- Gate: integration tests verify both expected failure and a successful subsequent clean run.

## M4: Qt diagnostics

- Show live run state, navigation state, sensor freshness, active faults, and recent events.
- Load two completed runs and compare outcomes and timing distributions.
- Gate: the UI remains responsive during a run and presents missing or partial data explicitly.

## M5: Evidence and reproducibility

- Add unit, contract, launch, restart, and scenario-level tests.
- Publish architecture and TF diagrams, benchmark tables, and demo media.
- Write a root-cause debugging narrative based on captured evidence.
- Document setup assumptions and nondeterminism sources without claiming perfect determinism.
- Gate: execute the quickstart on a clean supported host and archive the resulting report.

