<!--
Sync Impact Report
- Version change: 1.0.0 -> 1.1.0
- Modified principles: Engineering Constraints, simulator scope updated from
  Isaac Sim-only to Gazebo-only for the active project direction
- Added sections: none
- Removed sections: none
- Follow-up TODOs: validate the exact Gazebo release and ROS 2 Jazzy
  compatibility during feature research
-->

# ROS 2 Mobile Robot Profiling Lab Constitution

## Core Principles

### I. Measurable Behavior First

Every feature MUST define an observable robot behavior, a failure outcome, and measurable acceptance criteria before implementation. Metrics MUST identify units, clock domains, and aggregation rules. A successful demo without retained evidence is insufficient.

### II. ROS Contracts Are Explicit

Nodes MUST document topic, action, service, parameter, QoS, frame, and timestamp contracts that they own or consume. Frame IDs MUST belong to the documented TF tree. Code MUST NOT subtract timestamps from different clock domains, and simulated runs MUST respect `/clock` and `use_sim_time`.

### III. Test First

Tests MUST be written and observed failing before behavior is implemented. Pure logic receives unit tests; ROS interfaces receive contract or launch tests; task execution, fault injection, and restart behavior receive integration tests. A change is complete only when its focused tests and the repository validation command pass.

### IV. Faults Are Controlled and Visible

Injected delay or loss MUST be configured, bounded, timestamped, and included in run artifacts. Injection MUST occur at a named interface so source data remains available for comparison. The system MUST distinguish task failure, injected-fault response, and infrastructure failure.

### V. Reproducible, Not Perfectly Deterministic

Supported versions, scenario inputs, seeds when available, configuration hashes, and host assumptions MUST accompany published results. Reports MUST state known variation from physics, GPU scheduling, middleware, executors, and host load. Claims MUST be limited to evidence collected under the recorded configuration.

## Engineering Constraints

- Production ROS 2 application nodes MUST be C++20 and use standard ROS 2 and Nav2 interfaces directly.
- Gazebo is the only simulator in the active project scope. Isaac Sim is
  superseded and must not be introduced into active implementation work without
  a new approved specification.
- The active simulator integration MUST support the constrained-hardware
  execution profile defined by the current feature specification, including a
  documented headless or low-resource mode where applicable.
- The diagnostic desktop application MUST use Qt 6 and consume the same documented ROS and artifact contracts as automated tools.
- Run artifacts MUST use versioned, machine-readable formats and remain inspectable without the UI.
- Abstractions MUST solve a demonstrated boundary or duplication problem; speculative framework layers are prohibited.

## Development Workflow and Quality Gates

1. Evolve `spec.md` before changing promised behavior.
2. Resolve material unknowns in `research.md`; do not hide them in implementation tasks.
3. Re-check this constitution in `plan.md` after design changes.
4. Derive tasks from prioritized user stories and include exact target paths.
5. Implement in red-green-refactor order and retain focused validation evidence.
6. Run convergence analysis before declaring a feature complete.

Reviews MUST reject undocumented ROS contracts, mixed clock arithmetic, unbounded waits, unclassified failures, or performance claims without artifacts. Published benchmark changes require at least 20 trials per compared configuration unless the report clearly labels the result exploratory.

## Governance

This constitution governs specifications, plans, tasks, implementation, and review. Amendments require a documented rationale, an impact assessment for active specifications, and a version change following semantic versioning. Compliance MUST be checked during planning and convergence; justified exceptions MUST be recorded in the plan's Complexity Tracking section.

**Version**: 1.1.0 | **Ratified**: 2026-09-21 | **Last Amended**: 2026-09-23
