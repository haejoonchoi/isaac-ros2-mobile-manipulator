# Artifact Schema Contract

## Versioning

Every artifact includes `schema_version`, `run_id`, `scenario_id`,
`profile_id`, `created_at`, and a clock-segment identifier. Readers must
identify unsupported versions rather than silently accepting them.

## Trial Event

Each JSONL event contains:

```json
{
  "schema_version": 1,
  "run_id": "unique-run-id",
  "event_id": "unique-event-id",
  "event_type": "readiness|navigation|metric|fault|reset|resource|terminal",
  "sample_time": {"value": 0.0, "clock_domain": "ros_sim", "segment": 0},
  "source": "node-or-component",
  "payload": {}
}
```

Event records are append-only. A partial final line is ignored and reported as
incomplete; prior complete records remain readable.

## Terminal Record

The terminal record contains terminal status, failure class, duration with its
clock domain, shutdown outcome, resource observations, and references to all
event files. It is written atomically after the terminal event.

## Batch Summary

The summary contains exact trial count, classified trial count, success rate,
duration statistics, sensor timing statistics, failure-class counts, execution
profile, and limitations.

## Integrity and Compatibility

Artifacts include configuration hashes and simulator/host metadata. A reader
must distinguish missing fields, partial records, incompatible schema versions,
and valid zero-valued metrics.
