# Legacy baseline

The repository originally contained `mobile_manipulator_demo`, a small Python
ROS 2 package for a pick-and-place task state machine. Its useful baseline was:

- a minimal `ament_python` package layout;
- a task-state model with explicit transitions;
- a YAML task configuration;
- unit tests that exercised the state-machine behavior without requiring ROS 2.

That package is removed as part of the migration to the navigation-profiling
system. The new project intentionally does not preserve manipulation behavior:
the initial scope is one differential-drive navigation robot in NVIDIA Isaac
Sim, evaluated through Nav2 with recorded metrics and controlled sensor faults.

The legacy unit tests passed before removal and are not a validation target for
the new architecture.
