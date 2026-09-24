# Feature Specification: Navigation Profiling and Evaluation

**Feature Branch**: `001-navigation-profiling`

**Created**: 2026-09-21

**Status**: Superseded by [002-gazebo-integration](../002-gazebo-integration/spec.md)

> This Isaac Sim specification is retained as historical context. The project
> direction has moved to Gazebo because the available hardware cannot run
> Isaac Sim reliably. Do not use this feature for new planning or
> implementation work.

**Input**: Build a ROS 2 mobile-robot simulation with navigation tasks, automated profiling, controlled sensor failures, recovery behavior, and a Qt diagnostic interface.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Execute and Measure a Navigation Task (Priority: P1)

As a robotics engineer, I can run a defined navigation scenario repeatedly and receive evidence showing whether each trial succeeded and how it behaved.

**Why this priority**: A measured, repeatable task is the smallest useful system and the foundation for fault analysis and visualization.

**Independent Test**: From a clean simulation state, run one healthy scenario and a batch of trials; verify the robot reaches the goal and every trial produces a complete result and timing record.

**Acceptance Scenarios**:

1. **Given** a valid scenario and ready simulator, **When** the engineer starts one run, **Then** the robot attempts the configured goal and records start, feedback, terminal state, and duration.
2. **Given** a batch size of 20, **When** the batch completes, **Then** the report contains 20 classified trials plus success-rate and duration statistics.
3. **Given** a run artifact, **When** the engineer inspects it, **Then** its scenario, software versions, configuration identity, units, and clock domains are identifiable.

---

### User Story 2 - Observe Failure and Restart Behavior (Priority: P2)

As a robotics engineer, I can introduce a known sensor delay or outage and see how navigation fails, recovers, and behaves after restart.

**Why this priority**: Controlled faults turn a navigation demo into an engineering evaluation tool and expose timing and lifecycle behavior.

**Independent Test**: Run one scenario with a scheduled lidar delay and one with a scheduled lidar outage; verify the fault is recorded, the task exits within its deadline, and a subsequent healthy run starts without stale state.

**Acceptance Scenarios**:

1. **Given** a delay schedule, **When** its activation time arrives, **Then** affected messages are delayed by the configured amount and the observed interval is recorded.
2. **Given** an outage schedule, **When** messages are suppressed, **Then** the system identifies stale sensor data and reaches a bounded success, recovery, or failure outcome.
3. **Given** a failed or interrupted run, **When** the application restarts and a healthy run begins, **Then** no prior goal, fault, timer, or metric is attributed to the new run.

---

### User Story 3 - Diagnose and Compare Runs (Priority: P3)

As a robotics engineer, I can use a desktop interface to inspect a live run and compare completed runs without manually parsing logs.

**Why this priority**: The diagnostic interface is the portfolio differentiator, but it depends on trustworthy data produced by the first two stories.

**Independent Test**: Open the interface during a run, then load one healthy and one faulted artifact; verify live state and sensor freshness update and the comparison shows outcomes and timing differences.

**Acceptance Scenarios**:

1. **Given** an active run, **When** state, timing, or fault information changes, **Then** the interface displays the latest values without blocking robot execution.
2. **Given** two compatible run sets, **When** the engineer selects them, **Then** the interface compares success rate, duration, sensor timing, and failure classifications.
3. **Given** an incomplete or incompatible artifact, **When** it is opened, **Then** the interface identifies the missing or unsupported data instead of presenting misleading values.

### Edge Cases

- The simulator starts but required transforms or sensor streams never become ready.
- ROS time pauses, jumps backward during reset, or differs from host steady time.
- A goal is unreachable, rejected, canceled, or still active when its deadline expires.
- A fault interval crosses task completion or process restart.
- A delayed message arrives after a newer message.
- The simulator or a node exits mid-run, leaving a partial artifact.
- A batch is interrupted and later resumed.
- An artifact uses a newer schema version than the reader supports.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST execute a configured mobile-robot navigation goal in a simulated environment.
- **FR-002**: The system MUST publish and consume a documented, connected coordinate-frame tree for map, odometry, base, and sensor frames.
- **FR-003**: The system MUST validate required transforms, timestamps, and sensor freshness before starting a task.
- **FR-004**: The system MUST support single-run and repeated batch execution from version-controlled scenario definitions.
- **FR-005**: The system MUST reset robot, navigation, fault, and measurement state between trials.
- **FR-006**: The system MUST assign every trial a terminal status and a machine-readable failure classification.
- **FR-007**: The system MUST record task duration, sensor message age, sensor inter-arrival timing, navigation state, recovery events, and active fault intervals.
- **FR-008**: Every metric MUST include or inherit its unit and clock domain.
- **FR-009**: The system MUST inject configured delay and message loss without modifying the original source stream.
- **FR-010**: The system MUST enforce bounded readiness, task, cancellation, and shutdown deadlines.
- **FR-011**: The system MUST support restart after success, failure, cancellation, and unexpected process termination without reusing stale run state.
- **FR-012**: The system MUST write versioned event artifacts for each trial and a summary for each batch.
- **FR-013**: The system MUST provide a Qt diagnostic interface for live state, timing, faults, recent events, and comparison of completed runs.
- **FR-014**: The system MUST expose partial, missing, and incompatible data explicitly in reports and the interface.
- **FR-015**: The repository MUST include unit, ROS contract, launch/integration, restart, and scenario-level tests.
- **FR-016**: The repository MUST include reproducible setup instructions, architecture and frame diagrams, and an evidence-based debugging write-up.
- **FR-017**: Published evaluation reports MUST identify known nondeterminism and MUST NOT claim perfect determinism.

### Key Entities

- **Scenario Definition**: Versioned task, start state, goal, deadlines, fault schedule, reset policy, and optional seed.
- **Run Record**: Identity, environment metadata, lifecycle timestamps, terminal status, failure class, and links to events.
- **Metric Sample**: Named value or category with unit, clock domain, source, and sample time.
- **Fault Schedule**: Target stream, fault mode, activation interval, and parameters.
- **Run Set**: A collection of comparable runs and aggregate statistics.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: A new user following the supported setup can complete one healthy navigation run and locate its artifacts without editing source code.
- **SC-002**: A 20-trial batch produces exactly 20 terminal classifications and computes success rate and duration statistics from those records.
- **SC-003**: In automated fault tests, 100% of configured lidar fault intervals appear in artifacts with requested and observed timing.
- **SC-004**: Every task leaves the running state within its configured deadline plus a 2-second shutdown allowance.
- **SC-005**: A healthy run immediately following a failed run completes without events or metrics carrying the previous run ID.
- **SC-006**: The diagnostic interface refreshes visible live state at least twice per second, and its ROS worker processes synthetic 20 Hz diagnostic input with no dropped updates and a p95 callback duration of 10 ms or less.
- **SC-007**: The comparison view reports success rate, median task duration, and sensor timing summaries for two run sets of at least 20 trials each.
- **SC-008**: All published results include environment metadata and a limitations section naming at least simulator, middleware, scheduling, and host-load variability.

## Assumptions

- The initial platform is a Linux workstation capable of running the pinned Isaac Sim release and ROS 2 Jazzy.
- Navigation uses a differential-drive robot and lidar-based localization; manipulation is outside this feature.
- One operator runs local experiments; distributed orchestration and multi-robot scenarios are outside this feature.
- The initial controlled faults target lidar messages. Additional sensors require separate acceptance scenarios.
- Ground-truth data may be recorded for evaluation but is not used by navigation unless the scenario explicitly declares it.