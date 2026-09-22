# Artifact Contract

## Directory Layout

```text
artifacts/<batch-id>/
|-- batch.json
|-- summary.csv
`-- runs/<run-id>/
    |-- run.json
    |-- events.jsonl
    `-- recording/       # Optional rosbag2 output
```

## Compatibility Rules

- Every JSON object includes `schema_version`.
- Readers MUST reject unsupported major versions and MAY accept newer minor versions while ignoring unknown optional fields.
- Writers MUST create `run.json` with `artifact_state: partial` before navigation begins.
- Writers MUST append events in sequence order and atomically replace the terminal run record.
- Readers of an active `events.jsonl` MUST consume only newline-terminated objects and ignore an unterminated final object.
- Missing optional rosbag2 data MUST NOT make summary generation fail.
- CSV columns MUST be documented and stable within a schema major version.
- `configuration_hash` MUST be the lowercase SHA-256 digest of a manifest containing each configuration input's repository-relative path, byte length, and exact bytes, ordered lexicographically by path.

## Required Event Categories

- `run.lifecycle`
- `navigation.feedback`
- `navigation.recovery`
- `sensor.timing`
- `fault.requested`
- `fault.observed`
- `run.terminal`

## Validation

Contract tests MUST cover a complete successful artifact, a faulted artifact, a partial interrupted artifact, an unsupported major version, and unknown optional fields.