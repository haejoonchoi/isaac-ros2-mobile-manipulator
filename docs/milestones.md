# Project milestones

## M1: Simulation baseline

- Create Isaac Sim scene with mobile base, manipulator, table, bin, and graspable cube.
- Export or reference robot USD assets.
- Verify joint limits and collision geometry.

## M2: ROS2 bridge

- Enable Isaac ROS bridge.
- Publish `/joint_states`, `/tf`, odometry, and camera streams.
- Subscribe to velocity and arm trajectory commands.

## M3: MoveIt 2 planning

- Add URDF/Xacro and SRDF.
- Configure planning groups for arm and gripper.
- Add planning-scene collision objects for table, bin, and cube.

## M4: Pick-and-place executive

- Implement the sequence:
  1. Navigate to pick pose.
  2. Move to pre-grasp.
  3. Approach and close gripper.
  4. Lift object.
  5. Navigate to place pose.
  6. Lower, release, and retreat.

## M5: Validation

- Add tests for task-state transitions.
- Add launch/integration tests for ROS graph availability.
- Record task metrics: success rate, planning time, final object-pose error, collisions.

## M6: Portfolio presentation

- Add demo video/GIF.
- Add architecture diagram.
- Add trial-results table.
- Add concise engineering write-up: tradeoffs, failures, next steps.

