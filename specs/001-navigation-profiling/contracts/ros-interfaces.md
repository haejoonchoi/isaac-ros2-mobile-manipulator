# ROS Interface Contract

Final namespaces and message packages are confirmed during compatibility research. These logical contracts are stable requirements.

## Standard Interfaces

| Interface | Direction | Type | QoS / behavior |
| --- | --- | --- | --- |
| `/clock` | simulator to all nodes | `rosgraph_msgs/msg/Clock` | Keep last 1, best effort, volatile; all task nodes set `use_sim_time=true` |
| `/tf` | simulator/localization to consumers | `tf2_msgs/msg/TFMessage` | Keep last 100, reliable, volatile |
| `/tf_static` | robot description to consumers | `tf2_msgs/msg/TFMessage` | Keep last 1, reliable, transient local |
| `/scan_raw` | simulator to fault injector/metrics | `sensor_msgs/msg/LaserScan` | Keep last 5, best effort, volatile; never modified by injector |
| `/scan` | fault injector to Nav2/metrics | `sensor_msgs/msg/LaserScan` | Keep last 5, best effort, volatile; delayed or dropped according to active schedule |
| `/odom` | simulator to Nav2/metrics | `nav_msgs/msg/Odometry` | Keep last 5, best effort, volatile; stamp and frame IDs validated |
| `/imu/data` | simulator to metrics | `sensor_msgs/msg/Imu` | Keep last 5, best effort, volatile |
| `navigate_to_pose` | runner to Nav2 | `nav2_msgs/action/NavigateToPose` | One active scenario goal; cancellation is bounded |

## Project Interfaces

| Interface | Direction | Required content |
| --- | --- | --- |
| `~/run_events` | runner/collector to UI | Run ID, sequence, event type, timestamp with domain, source, payload |
| `~/fault_events` | injector to collector/UI | Run ID, fault ID, requested interval, observed interval, target, mode |
| `~/run_control` | CLI/test to runner | Start or cancel a named scenario and optional batch identity |
| `~/run_state` | runner to collector/UI | Run ID, lifecycle state, scenario, terminal status, failure class |

Project-specific messages MUST live in `navigation_evaluation` unless a dedicated interface package becomes necessary to break a demonstrated dependency cycle.

## Frame Contract

```text
map -> odom -> base_link -> lidar_link
                         -> imu_link
```

- Localization owns `map -> odom`.
- Odometry owns `odom -> base_link`.
- Robot description owns static sensor transforms.
- Scenario start and goal poses use `map` unless explicitly documented otherwise.
- Readiness requires connected transforms at current ROS time within configured freshness bounds.

## Timestamp Contract

- Sensor age is computed only from ROS time and message header stamps.
- Task deadlines and process shutdown use steady time.
- System time is metadata only.
- Reset-induced ROS-time jumps close the current measurement interval; samples MUST NOT span the jump.
- Compatibility research MUST verify simulator publishers can match these profiles before implementation; any required change updates this contract and its launch tests together.