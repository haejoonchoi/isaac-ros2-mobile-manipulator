# Isaac Sim + ROS2 Mobile Manipulator

Portfolio project for a robotics role: a simulated mobile manipulator in NVIDIA Isaac Sim, controlled through ROS2 and MoveIt 2, performing a validated pick-and-place task.

## Goal

Demonstrate end-to-end robotics engineering skills:

- Build a mobile manipulator simulation scene in Isaac Sim.
- Connect Isaac Sim to ROS2 through the ROS bridge.
- Plan arm motion with MoveIt 2.
- Coordinate base navigation, grasping, placement, and validation.
- Publish repeatable tests and short demo media for reviewers.

## Target stack

| Area | Tooling |
| --- | --- |
| Simulator | NVIDIA Isaac Sim |
| Middleware | ROS2 Humble or Jazzy |
| Motion planning | MoveIt 2 |
| Robot model | URDF/Xacro plus Isaac USD scene |
| Validation | Python unit tests, ROS2 launch tests, task-level metrics |

## Repository layout

```text
.
├── docs/                         # Design notes, milestones, demo checklist
├── docker/                       # Optional ROS2 development container
├── ros2_ws/
│   └── src/
│       └── mobile_manipulator_demo/
│           ├── config/           # Task and robot configuration
│           ├── launch/           # ROS2 launch files
│           ├── mobile_manipulator_demo/
│           │   └── task_state_machine.py
│           ├── test/             # Validation tests
│           ├── package.xml
│           └── setup.py
└── scripts/                      # Helper commands
```

## Milestones

1. **Simulation scene**: load mobile base, arm, table, objects, and cameras in Isaac Sim.
2. **ROS2 bridge**: publish joint states, TF, odometry, camera, and command topics.
3. **MoveIt 2 planning**: configure arm kinematics, planning scene, and collision objects.
4. **Task executive**: sequence navigate, detect/object-pose input, pre-grasp, grasp, lift, place.
5. **Validation**: assert task success, collision-free plans, final object pose tolerance, and repeatability.
6. **Portfolio polish**: add architecture diagram, demo video/GIF, benchmark table, and short write-up.

## Quick start

From the repository root:

```powershell
cd ros2_ws
colcon build --symlink-install
. install/setup.ps1
ros2 launch mobile_manipulator_demo demo.launch.py
```

For a non-ROS smoke check of the task logic:

```powershell
python -m unittest discover ros2_ws\src\mobile_manipulator_demo\test
```

## Public portfolio checklist

- Include a 60-90 second demo video in the README.
- Show a task-success table with at least 20 trials.
- Add screenshots of Isaac Sim, RViz/MoveIt, and the ROS graph.
- Document failure modes and what you would improve next.
- Keep hardware claims simulated unless tested on real hardware.

