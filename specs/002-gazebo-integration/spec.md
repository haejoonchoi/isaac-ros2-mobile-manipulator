# Feature Specification: Gazebo Integration for Constrained Hardware

**Feature Branch**: `002-gazebo-integration`

**Created**: 2026-09-23

**Status**: Active draft; supersedes [001-navigation-profiling](../001-navigation-profiling/spec.md)

> This feature is now the project's active simulator direction. The Isaac Sim
> feature is retained only as superseded historical context.

**Input**: User description: "Use Gazebo instead of Isaac Sim because the available computer hardware cannot run Isaac Sim reliably, and update the project definitions for the new simulator integration."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Run a Healthy Navigation Scenario on Gazebo (Priority: P1)

As a robotics engineer with modest workstation hardware, I can start the supported Gazebo simulation and run a repeatable mobile-robot navigation scenario without using Isaac Sim.

**Why this priority**: A reliable, lower-resource simulation is the minimum viable replacement for the blocked Isaac Sim workflow.

**Independent Test**: On a supported Ubuntu 24.04 workstation, start the simulation in the documented low-resource mode, execute one healthy navigation run, and verify that the robot reaches the goal and produces a complete run artifact.

**Acceptance Scenarios**:

1. **Given** a supported workstation and a valid healthy scenario, **When** the engineer starts the documented simulation command, **Then** the simulator, robot, required sensors, coordinate frames, clock, localization, and navigation services become ready without Isaac Sim installed.
2. **Given** a ready simulation, **When** the engineer starts a navigation run, **Then** the robot attempts the configured goal and records start time, feedback, terminal status, duration, and configuration identity.
3. **Given** a supported workstation running without hardware-accelerated rendering, **When** the engineer runs the healthy scenario, **Then** the scenario remains executable in headless or low-resource mode and reports any unsupported visual features explicitly.

---

### User Story 2 - Measure Repeatability and Resource Use (Priority: P2)

As a robotics engineer, I can repeat navigation trials and understand whether the Gazebo setup stays within the computer's available resources.

**Why this priority**: The replacement is valuable only if it is stable enough for measurement and does not reproduce the original hardware problem.

**Independent Test**: Run a batch of 20 healthy trials in the documented execution mode, inspect the resulting statistics, and verify that the run records include host and simulator resource observations.

**Acceptance Scenarios**:

1. **Given** a valid scenario and batch size of 20, **When** the batch completes, **Then** the report contains exactly 20 terminal trial classifications plus success-rate and duration statistics.
2. **Given** a constrained workstation, **When** a run or batch approaches a configured CPU, memory, or simulation-progress limit, **Then** the system records the condition and ends with a classified outcome rather than hanging.
3. **Given** two batches run with the same scenario definition, **When** the engineer compares them, **Then** the report identifies the simulator, world, robot model, scenario version, host assumptions, and known sources of variation.

---

### User Story 3 - Exercise Faults and Restart Safely (Priority: P3)

As a robotics engineer, I can inject controlled lidar delay or outage in Gazebo-backed navigation runs and restart cleanly after failure.

**Why this priority**: Controlled faults and restart behavior are necessary for the existing evaluation goals, but depend on a working Gazebo baseline.

**Independent Test**: Run one lidar-delay scenario and one lidar-outage scenario, interrupt or fail a run, then start a healthy run and verify fault timing, bounded termination, and run-state isolation.

**Acceptance Scenarios**:

1. **Given** a configured delay schedule, **When** the activation interval begins, **Then** affected lidar messages are delayed by the requested amount, raw messages remain available, and requested versus observed timing is recorded.
2. **Given** a configured outage schedule, **When** messages are suppressed, **Then** stale sensor data is detected and the task reaches a bounded success, recovery, or failure outcome.
3. **Given** a failed, canceled, or interrupted run, **When** a new healthy run starts, **Then** no goal, fault, timer, metric, or event from the prior run is attributed to the new run.

---

### User Story 4 - Diagnose and Compare Gazebo Runs (Priority: P4)

As a robotics engineer, I can inspect live state and compare healthy and faulted Gazebo runs without manually parsing logs.

**Why this priority**: Diagnostics turn the simulator replacement into a useful evaluation environment and preserve the project’s original measurement goals.

**Independent Test**: Open the diagnostic interface during a run, then compare two completed 20-trial run sets and open an incomplete artifact.

**Acceptance Scenarios**:

1. **Given** an active run, **When** navigation, timing, sensor freshness, or fault state changes, **Then** the interface displays the latest values without blocking simulation or navigation execution.
2. **Given** two compatible run sets, **When** the engineer selects them, **Then** the interface compares success rate, median duration, sensor timing, and failure classifications.
3. **Given** an incomplete or unsupported artifact, **When** the engineer opens it, **Then** the interface identifies missing or unsupported data instead of displaying misleading values.

### Edge Cases

- Gazebo starts but required models, plugins, transforms, or sensor streams never become ready.
- The workstation cannot sustain the configured real-time factor or exceeds the configured resource budget.
- The simulation clock pauses, jumps backward during reset, or stops when the simulator is paused.
- A robot spawn request fails or creates duplicate entities after restart.
- A goal is unreachable, rejected, canceled, or active when its deadline expires.
- A fault interval crosses task completion or process restart.
- A delayed message arrives after a newer message.
- The simulator or a node exits mid-run, leaving a partial artifact.
- A batch is interrupted and later resumed.
- An artifact uses a newer schema version than the reader supports.
- Headless mode is requested but a required visual or GUI capability is unavailable.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST provide a supported Gazebo-based simulation path for the mobile robot without requiring Isaac Sim.
- **FR-002**: The system MUST document the supported simulator, ROS distribution, operating system, robot model, world, and execution modes.
- **FR-003**: The system MUST support graphical and headless or low-resource execution modes, with unsupported capabilities reported explicitly.
- **FR-004**: The system MUST publish and consume a documented, connected coordinate-frame tree for map, odometry, base, and sensor frames.
- **FR-005**: The system MUST provide simulation time consistently to participating nodes and identify the clock domain of recorded measurements.
- **FR-006**: The system MUST validate required models, transforms, timestamps, sensor freshness, navigation services, and clock progress before starting a task.
- **FR-007**: The system MUST support single-run and repeated batch execution from version-controlled scenario definitions.
- **FR-008**: The system MUST reset the robot, navigation, fault, and measurement state between trials without leaving duplicate entities or stale state.
- **FR-009**: The system MUST assign every trial a terminal status and a machine-readable failure classification, including simulator and resource-limit failures.
- **FR-010**: The system MUST record task duration, sensor message age, sensor inter-arrival timing, navigation state, recovery events, active fault intervals, simulator identity, execution mode, and host resource observations.
- **FR-011**: Every metric MUST include or inherit its unit, clock domain, source, and sample time.
- **FR-012**: The system MUST inject configured delay and message loss at a named relay boundary without modifying the original source stream.
- **FR-013**: The system MUST enforce bounded readiness, task, cancellation, shutdown, and simulation-progress deadlines.
- **FR-014**: The system MUST support restart after success, failure, cancellation, and unexpected process termination without reusing stale run state.
- **FR-015**: The system MUST write versioned, machine-readable event artifacts for each trial and a summary for each batch that remain inspectable without the diagnostic interface.
- **FR-016**: The system MUST provide a Qt diagnostic interface for live state, timing, faults, recent events, partial data, and comparison of completed runs.
- **FR-017**: The repository MUST include unit, ROS contract, launch/integration, restart, scenario-level, and constrained-hardware smoke tests.
- **FR-018**: The repository MUST include reproducible setup instructions, architecture and frame diagrams, a resource-budget profile, and an evidence-based debugging write-up.
- **FR-019**: Published evaluation reports MUST identify known nondeterminism and MUST NOT claim perfect determinism.

### Key Entities

- **Gazebo Execution Profile**: Supported operating-system assumptions, simulator and ROS versions, graphical or headless mode, resource limits, and expected simulation-progress behavior.
- **Scenario Definition**: Versioned task, start state, goal, deadlines, fault schedule, reset policy, execution profile, and optional seed.
- **Run Record**: Identity, environment metadata, execution mode, resource observations, lifecycle timestamps, terminal status, failure class, and links to events.
- **Metric Sample**: Named value or category with unit, clock domain, source, and sample time.
- **Fault Schedule**: Target stream, fault mode, activation interval, and parameters.
- **Run Set**: A collection of comparable runs and aggregate statistics.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: A new user following the supported setup can start a Gazebo simulation and complete one healthy navigation run without installing or launching Isaac Sim.
- **SC-002**: On the documented minimum workstation profile, the healthy scenario reaches readiness within 120 seconds and completes a single task within its configured deadline plus a 2-second shutdown allowance.
- **SC-003**: A 20-trial batch produces exactly 20 terminal classifications and computes success rate and duration statistics from those records.
- **SC-004**: The supported low-resource or headless mode completes at least 19 of 20 healthy trials on the reference workstation, or records a classified simulator/resource failure for each unsuccessful trial.
- **SC-005**: During the reference smoke test, the simulation reports measurable forward clock progress and no required node uses a timestamp from an undocumented clock domain.
- **SC-006**: In automated fault tests, 100% of configured lidar fault intervals appear in artifacts with requested and observed timing.
- **SC-007**: A healthy run immediately following a failed or interrupted run completes without events or metrics carrying the previous run ID.
- **SC-008**: The diagnostic interface refreshes visible live state at least twice per second, processes synthetic 20 Hz diagnostic input with no dropped updates, and maintains a p95 callback duration of 10 ms or less.
- **SC-009**: The comparison view reports success rate, median task duration, and sensor timing summaries for two run sets of at least 20 trials each.
- **SC-010**: Every published result includes simulator, world, robot, execution-mode, host, and resource metadata plus a limitations section naming simulator, middleware, scheduling, and host-load variability.

## Assumptions

- The initial supported runtime is Ubuntu 24.04 with ROS 2 Jazzy and the Gazebo release officially paired with that ROS distribution.
- Gazebo is used as the sole simulator for this feature; Isaac Sim remains outside this feature’s runtime path.
- The reference workstation is a modest developer computer with at least 4 physical CPU cores, 16 GB RAM, and no requirement for a high-end discrete GPU; exact measured limits are established during planning research.
- The initial robot is differential-drive with lidar-based localization; manipulation is outside this feature.
- One operator runs local experiments; distributed orchestration and multi-robot scenarios are out of scope.
- The initial controlled faults target lidar messages. Additional sensors require separate acceptance scenarios.
- A headless or low-resource profile may reduce visual fidelity, but it must preserve navigation-relevant physics, sensors, timing, and artifacts.
- Ground-truth data may be recorded for evaluation but is not used by navigation unless the scenario explicitly declares it.
- Existing navigation evaluation, fault injection, artifact, and Qt diagnostic behavior is reused where it remains simulator-neutral.
