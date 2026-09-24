# Data Model: Navigation Profiling and Evaluation

## Terminology

- A **trial** is one attempted scenario execution.
- A **run record** is the persisted artifact describing one trial.
- A **batch** is an ordered collection of trials with one shared scenario and configuration.

## Scenario Definition

| Field | Type | Rules |
| --- | --- | --- |
| `schema_version` | string | Required semantic version |
| `scenario_id` | string | Stable, filesystem-safe identifier |
| `description` | string | Human-readable purpose |
| `start_pose` | pose | Frame ID plus planar position and yaw |
| `goal_pose` | pose | Frame ID plus planar position and yaw |
| `readiness_timeout_s` | number | Positive steady-time duration |
| `task_timeout_s` | number | Positive steady-time duration |
| `reset_policy` | enum | `world`, `robot`, or `navigation` combination |
| `faults` | list | Zero or more fault schedules |
| `seed` | integer/null | Recorded even when a subsystem cannot honor it |

## Fault Schedule

| Field | Type | Rules |
| --- | --- | --- |
| `fault_id` | string | Unique within a scenario |
| `target_stream` | string | Logical stream name, initially `lidar` |
| `mode` | enum | `delay` or `drop` |
| `start_offset_s` | number | Non-negative offset from task start |
| `duration_s` | number | Positive duration |
| `delay_ms` | number/null | Required and non-negative for delay mode |

Scenario files MUST use lowercase snake-case names ending in `.yaml`. The logical stream `lidar` maps to `/scan_raw` as its preserved source and `/scan` as its impaired navigation input.

## Run Record

| Field | Type | Rules |
| --- | --- | --- |
| `schema_version` | string | Reader rejects unsupported major versions |
| `run_id` | UUID | Unique across batches |
| `batch_id` | UUID/null | Shared by repeated runs |
| `scenario_id` | string | References scenario definition |
| `configuration_hash` | string | Hash of normalized scenario and runtime config |
| `environment` | object | ROS, simulator, package, asset, OS, and GPU versions |
| `started_at` | timestamp | Domain included |
| `duration_ms` | integer/null | Steady-clock elapsed duration |
| `terminal_status` | enum | `succeeded`, `failed`, `canceled`, `interrupted` |
| `failure_class` | enum/null | Readiness, navigation, stale sensor, timeout, infrastructure, or internal |
| `artifact_state` | enum | `partial` or `complete` |
| `events_file` | path | Relative path to JSONL events |

State transitions: `partial` is written atomically before task execution; exactly one atomic replacement moves the record to `complete`. An interrupted process leaves a valid `partial` record for diagnosis. Event readers may observe only complete newline-terminated JSONL entries while a writer is active and MUST ignore an unterminated final entry.

## Run Event

| Field | Type | Rules |
| --- | --- | --- |
| `run_id` | UUID | Must match owning run |
| `sequence` | integer | Strictly increasing within one run |
| `event_type` | string | Versioned event category |
| `observed_at` | timestamp | Value plus explicit clock domain |
| `source` | string | Owning node or subsystem |
| `payload` | object | Event-specific, schema-validated content |

## Metric Sample

| Field | Type | Rules |
| --- | --- | --- |
| `name` | string | Stable metric identifier |
| `value` | number/string | Numeric or categorical value |
| `unit` | string/null | Required for numeric values |
| `clock_domain` | enum/null | `ros`, `steady`, or `system`; required for time values |
| `sample_time` | timestamp | Domain included |
| `source` | string | Topic, node, or derived calculation |

## Run Set Summary

| Field | Type | Rules |
| --- | --- | --- |
| `batch_id` | UUID | Batch identity |
| `scenario_id` | string | Shared scenario |
| `configuration_hash` | string | Shared comparison boundary |
| `trial_count` | integer | Includes every attempted trial |
| `status_counts` | map | Sum equals trial count |
| `success_rate` | number | Successful trials divided by trial count |
| `duration_statistics` | object | Count, median, p90, min, max in milliseconds |
| `sensor_statistics` | object | Age and inter-arrival summaries with units |

Two run sets are directly comparable only when schema major versions and declared metric semantics match. Configuration differences MUST be displayed rather than silently merged.