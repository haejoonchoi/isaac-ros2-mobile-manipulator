# Data Model: Gazebo Integration

## Gazebo Execution Profile

Describes a supported runtime configuration.

| Field | Description | Validation |
|---|---|---|
| `profile_id` | Stable profile name, such as `headless-low-resource` | Required, version-controlled |
| `simulator` | Gazebo distribution and release | Must be Gazebo Harmonic |
| `ros_distribution` | ROS distribution | Must be Jazzy |
| `mode` | `server_only`, `headless_rendering`, or `graphical` | Mode capabilities are explicit |
| `cpu_cores_min` | Minimum physical cores | At least 4 for reference profile |
| `memory_gb_min` | Minimum RAM | At least 16 for reference profile |
| `readiness_timeout_s` | Maximum startup readiness duration | 120 seconds or less |
| `clock_timeout_s` | Maximum time to observe clock progress | 20 seconds or less |

## Scenario Definition

Versioned navigation input containing start pose, goal pose, deadlines, reset
policy, fault schedule, execution profile, and optional seed. Scenario
validation rejects missing goals, unsupported profiles, invalid deadlines,
unknown fault modes, and incompatible schema versions.

## Run Record

Identifies one trial and its lifecycle.

| Field | Description |
|---|---|
| `run_id` | Unique identifier that is never reused |
| `scenario_id` / `scenario_version` | Scenario identity |
| `profile_id` / `profile_version` | Execution profile identity |
| `simulator_identity` | Gazebo and bridge versions |
| `host_metadata` | OS, CPU, memory, rendering capability |
| `lifecycle` | Readiness, start, feedback, terminal, shutdown timestamps |
| `terminal_status` | Success, failure, canceled, timeout, or interrupted |
| `failure_class` | Machine-readable task, simulator, resource, fault-response, or infrastructure class |
| `resource_observations` | CPU, memory, clock progress, and optional real-time factor |
| `event_links` | Paths to append-only event records |

## Metric Sample

A named numeric or categorical observation with `value`, `unit`,
`clock_domain`, `source`, and `sample_time`. ROS-time samples cannot be
subtracted from steady-time samples. Reset boundaries create a new clock
segment.

## Fault Schedule

Defines target stream, delay or loss mode, activation interval, requested
parameters, and observed timing. Raw source messages remain available.

## Run Set

A collection of compatible run records with scenario/profile identity and
aggregate success rate, median duration, sensor timing, and failure-class
statistics.

## State Transitions

```text
created -> starting -> ready -> running -> terminal -> archived
                         |         |
                         v         v
                      failed   interrupted
```

Reset is a bounded transition from `terminal`, `failed`, or `interrupted` to a
new `created` run. A new run must receive a new `run_id` and clock segment.
