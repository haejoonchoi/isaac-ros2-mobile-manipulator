# Quickstart Validation: Gazebo Integration

This guide validates the required low-resource path. It is intentionally a
run guide; implementation details belong in `tasks.md`.

## Prerequisites

- Ubuntu 24.04 with ROS 2 Jazzy.
- Gazebo Harmonic and the `ros_gz` packages.
- At least four physical CPU cores, 16 GB RAM, and 5 GB free disk for the
  reference profile.
- A built workspace with the four project packages.

## Build

```bash
source /opt/ros/jazzy/setup.bash
cd ros2_ws
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
```

## Healthy Headless Run

Start the documented `headless-low-resource` profile. The launch must start
Gazebo Harmonic server-only, bridge `/clock`, spawn exactly one robot, bring up
localization and Nav2, and wait for the readiness contract in
[`contracts/ros-interfaces.md`](contracts/ros-interfaces.md).

Run the healthy scenario and inspect the generated artifact using the schema in
[`contracts/artifact-schema.md`](contracts/artifact-schema.md).

Expected evidence:

- `/clock` advances within 20 seconds;
- readiness completes within 120 seconds;
- the TF chain is connected;
- one terminal record and complete event stream are written;
- no Isaac Sim process or package is required.

## Batch and Fault Validation

Run the healthy scenario with a batch size of 20 and verify exactly 20
terminal classifications. Then run the lidar-delay and lidar-outage scenarios,
followed by a healthy run after reset or interruption.

Expected evidence:

- requested and observed fault intervals are present;
- raw and impaired lidar remain distinguishable;
- reset leaves one robot entity and no prior run ID in the new artifact;
- simulator/resource failures are classified rather than hanging.

## Resource Evidence

Record execution mode, CPU, memory, simulator identity, clock progress, and
known host-load variation with every published result. Do not treat a real-time
factor of 1.0 as a requirement for smoke tests; report measured progress and
label exploratory results clearly.
