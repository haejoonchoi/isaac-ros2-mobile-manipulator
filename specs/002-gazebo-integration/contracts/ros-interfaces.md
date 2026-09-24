# ROS Interfaces Contract

## Clock and Time

- Gazebo is the sole publisher of `/clock`.
- Bridge direction is Gazebo to ROS for `rosgraph_msgs/msg/Clock`.
- All participating nodes declare `use_sim_time: true`.
- `override_timestamps_with_wall_time` remains disabled.
- A reset clock rewind is an explicit boundary and is recorded before any new
  trial metrics are accepted.

## Topics

| Topic | Type | Direction | QoS / Contract |
|---|---|---|---|
| `/scan_raw` | `sensor_msgs/msg/LaserScan` | bridge to ROS | Sensor-data QoS; source stream preserved |
| `/scan` | `sensor_msgs/msg/LaserScan` | fault relay to Nav2 | Sensor-data QoS; delayed/dropped copy |
| `/odom` | `nav_msgs/msg/Odometry` | bridge to ROS | Sim-time stamps; one authoritative odometry source |
| `/imu/data` | `sensor_msgs/msg/Imu` | bridge to ROS | Sensor-data QoS; sim-time stamps; authoritative source; `frame_id=imu_link` |
| `/tf` | `tf2_msgs/msg/TFMessage` | bridge/nodes to ROS | `map -> odom -> base_link` connectivity |
| `/tf_static` | `tf2_msgs/msg/TFMessage` | robot state publisher to ROS | Static sensor frames |
| `/clock` | `rosgraph_msgs/msg/Clock` | bridge to ROS | Exactly one publisher; strictly advances during a run |
| `/diagnostics` | project diagnostic message | evaluation to UI | Immutable snapshot input |

## Services

| Service | Purpose |
|---|---|
| `/world/<world>/control` | Pause and reset world state |
| `/world/<world>/create` | Spawn one named robot entity |
| `/world/<world>/remove` | Delete the prior robot entity |
| Nav2 lifecycle services | Controlled bring-up and restart |
| Nav2 costmap clear services | Remove stale costmap state after reset |

## Frames

The required connected chain is:

```text
map -> odom -> base_link -> lidar_link
                         `-> imu_link
```

There must be one authoritative publisher for each dynamic edge. Sensor frame
IDs must match the robot model and bridge configuration.

## Readiness Contract

Before a task starts, the runner verifies:

1. exactly one `/clock` publisher and advancing simulation time;
2. connected required TF chain;
3. raw and impaired lidar streams with compatible QoS;
4. fresh `/imu/data` messages with the expected `imu_link` frame and compatible QoS;
5. localization and Nav2 lifecycle readiness;
6. reset/entity-management services;
7. selected execution profile and resource assumptions.

All readiness checks have bounded deadlines and produce a classified failure.
