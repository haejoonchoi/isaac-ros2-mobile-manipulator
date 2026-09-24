# Research: Gazebo Integration for Constrained Hardware

## Simulator and ROS Distribution

**Decision**: Use Gazebo Harmonic with ROS 2 Jazzy on Ubuntu 24.04.

**Rationale**: Harmonic is the supported Gazebo pairing for ROS 2 Jazzy and
provides current binary packages for Ubuntu 24.04. Gazebo Classic does not
provide the same supported Jazzy path.

**Alternatives considered**: Gazebo Classic was rejected because it is an
older integration path without the supported Jazzy binary pairing. Isaac Sim
was rejected because the available workstation cannot run it reliably and it
is superseded by the active feature.

## ROS-Gazebo Boundary

**Decision**: Use `ros_gz_sim`, `ros_gz_bridge`, and
`ros_gz_interfaces`. Bridge `/clock`, lidar, odometry, IMU, and required
services with a checked-in YAML configuration.

**Rationale**: The `ros_gz` family is the standard Harmonic integration boundary
and keeps Nav2 on standard ROS messages. Fault injection remains a separate ROS
relay after the raw lidar bridge.

**Alternatives considered**: `gazebo_ros_pkgs` was rejected because it targets
the Gazebo Classic integration model rather than the selected Harmonic path.

## Headless and Resource Profile

**Decision**: Make server-only execution the required profile. Use
`gz sim -s -r` for physics-only worlds and `--headless-rendering` with OGRE2
only when a sensor requires rendering.

**Rationale**: Physics is primarily CPU-bound and does not require a discrete
GPU. Avoiding a GUI lowers the hardware floor. Headless rendering can fall back
to software rendering but is not appropriate for camera-heavy scenarios.

**Reference profile**: At least four physical CPU cores, 16 GB RAM, and 5–10 GB
free disk. The feature treats these as planning assumptions, not vendor SLAs.

**Measured smoke thresholds**: `/clock` must advance within 20 seconds of world
load; healthy readiness must complete within 120 seconds. Dedicated profiling
may measure real-time factor, but smoke tests require forward progress rather
than a real-time factor of 1.0.

**Alternatives considered**: A full GUI-first profile was rejected because it
adds an unnecessary X/Qt/rendering dependency to the constrained baseline.

## Clock and TF Semantics

**Decision**: Gazebo is the sole `/clock` publisher. All ROS nodes use
`use_sim_time=true`. The canonical TF chain is
`map -> odom -> base_link -> sensor_frames`.

**Rationale**: A single simulation clock prevents mixed timestamp domains.
Reset-induced clock rewinds are explicit lifecycle boundaries and must not be
used in cross-boundary duration arithmetic.

**Alternatives considered**: Wall-time stamping for bridged sensor messages was
rejected because it would invalidate sensor age and inter-arrival metrics.

## Reset and Restart

**Decision**: Reset trials with explicit delete, world reset, respawn,
localization/odometry reset, costmap clearing, and duplicate-entity checks.

**Rationale**: A world reset alone may leave dynamically spawned entities or
stale navigation state. Explicit entity management makes run isolation
observable and testable.

**Alternatives considered**: World-reset-only was rejected because it cannot
guarantee clean entity and Nav2 state across trials.

## Sources

- Gazebo Harmonic ROS 2 vendor packages:
  https://gazebosim.org/docs/harmonic/ros2_gz_vendor_pkgs/
- `ros_gz` Jazzy bridge documentation:
  https://github.com/gazebosim/ros_gz/tree/jazzy/ros_gz_bridge
- Gazebo headless rendering:
  https://gazebosim.org/api/sim/9/headless_rendering.html
- Gazebo ROS 2 simulation interfaces:
  https://gazebosim.org/docs/latest/ros2_sim_interfaces/
