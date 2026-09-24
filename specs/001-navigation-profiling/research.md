# Research: Navigation Profiling and Evaluation

> **Superseded**: This research records the abandoned Isaac Sim direction.
> The project now uses the Gazebo feature in
> `../002-gazebo-integration/spec.md`; new compatibility research belongs
> there.

## Simulator and Platform

**Historical decision**: Use NVIDIA Isaac Sim as the only simulator and ROS 2 Jazzy on Ubuntu 24.04.

**Rationale**: The repository and intended portfolio direction already target Isaac Sim. Restricting the first release to one simulator keeps bridge, timing, reset, and sensor semantics testable.

**Alternatives considered**: Gazebo offers lighter deployment and strong ROS integration, but supporting it now would double scenario and timing validation. It can be evaluated in a later specification.

**Validation still required**: Pin an exact Isaac Sim release only after a smoke test confirms its ROS 2 Jazzy bridge, GPU driver, and robot asset compatibility.

**Migration status**: The repository now targets native Ubuntu 24.04 as the
primary runtime. Docker remains an optional development environment; its base
image is aligned with ROS 2 Jazzy, but the image is not yet an Isaac Sim
runtime and must not be treated as a compatibility validation result.

## Diagnostic Toolkit

**Decision**: Use Qt 6 with ROS callbacks handled outside the GUI thread.

**Rationale**: Qt provides mature plotting, tables, file dialogs, testing, and desktop packaging. It supports a polished diagnostic application without building a rendering framework.

**Alternatives considered**: Dear ImGui has lower ceremony and excellent immediate-mode tooling, but Qt better fits persistent run comparisons and accessible desktop widgets.

## Fault Injection Boundary

**Decision**: Place a C++ relay between raw simulator lidar and the navigation input topic.

**Rationale**: The relay preserves original data, makes fault ownership explicit, allows healthy and impaired timestamps to be compared, and avoids modifying simulator or Nav2 internals.

**Alternatives considered**: Network shaping cannot target one ROS stream precisely; simulator modification entangles faults with scene assets; Nav2 modification invalidates comparisons with stock behavior.

## Clock Policy

**Decision**: Use ROS time for simulated event ordering and message age, and `std::chrono::steady_clock` for host elapsed durations and deadlines. Store the domain with every metric.

**Rationale**: Simulation time can pause or jump during reset, while steady time is monotonic but cannot be compared directly with message stamps.

**Alternatives considered**: Using ROS time for every duration makes host deadlines unsafe during pauses. Using wall time for sensor age mixes domains and produces invalid values.

## Evidence Storage

**Decision**: Store append-only JSONL events per trial, a terminal run record, CSV batch summaries, and optional rosbag2 data.

**Rationale**: JSONL survives partial runs and is easy to inspect; CSV serves quick comparison; rosbag2 preserves high-volume evidence without becoming the only source of truth.

**Alternatives considered**: SQLite adds migration and locking concerns before query volume justifies it. CSV alone cannot represent event details safely.

## Reproducibility Position

**Decision**: Promise versioned scenarios and statistically comparable repeated runs, not bit-for-bit determinism.

**Rationale**: Physics stepping, GPU work, DDS delivery, executors, and host load can vary even with fixed scenario inputs. Recording versions, seeds, and distributions supports honest engineering conclusions.