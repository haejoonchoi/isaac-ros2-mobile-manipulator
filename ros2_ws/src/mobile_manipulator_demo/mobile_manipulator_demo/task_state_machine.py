from dataclasses import dataclass
from enum import Enum


class TaskState(str, Enum):
    IDLE = "idle"
    NAVIGATING_TO_PICK = "navigating_to_pick"
    PLANNING_GRASP = "planning_grasp"
    GRASPING = "grasping"
    LIFTING = "lifting"
    NAVIGATING_TO_PLACE = "navigating_to_place"
    PLACING = "placing"
    COMPLETE = "complete"
    FAILED = "failed"


@dataclass(frozen=True)
class TaskObservation:
    base_at_pick: bool = False
    grasp_plan_ready: bool = False
    object_secured: bool = False
    lift_complete: bool = False
    base_at_place: bool = False
    object_released: bool = False
    fault: str | None = None


class PickPlaceStateMachine:
    def __init__(self) -> None:
        self.state = TaskState.IDLE

    def start(self) -> TaskState:
        if self.state != TaskState.IDLE:
            raise RuntimeError(f"Cannot start task from {self.state.value}")
        self.state = TaskState.NAVIGATING_TO_PICK
        return self.state

    def step(self, observation: TaskObservation) -> TaskState:
        if observation.fault:
            self.state = TaskState.FAILED
            return self.state

        if self.state == TaskState.NAVIGATING_TO_PICK and observation.base_at_pick:
            self.state = TaskState.PLANNING_GRASP
        elif self.state == TaskState.PLANNING_GRASP and observation.grasp_plan_ready:
            self.state = TaskState.GRASPING
        elif self.state == TaskState.GRASPING and observation.object_secured:
            self.state = TaskState.LIFTING
        elif self.state == TaskState.LIFTING and observation.lift_complete:
            self.state = TaskState.NAVIGATING_TO_PLACE
        elif self.state == TaskState.NAVIGATING_TO_PLACE and observation.base_at_place:
            self.state = TaskState.PLACING
        elif self.state == TaskState.PLACING and observation.object_released:
            self.state = TaskState.COMPLETE

        return self.state


def run_ros_node() -> None:
    import rclpy
    from rclpy.node import Node

    class TaskStateMachineNode(Node):
        def __init__(self) -> None:
            super().__init__("task_state_machine")
            self.machine = PickPlaceStateMachine()
            self.machine.start()
            self.timer = self.create_timer(1.0, self._tick)
            self.get_logger().info(f"Task started: {self.machine.state.value}")

        def _tick(self) -> None:
            self.get_logger().info(f"Current task state: {self.machine.state.value}")

    rclpy.init()
    node = TaskStateMachineNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


def main() -> None:
    run_ros_node()


if __name__ == "__main__":
    main()

