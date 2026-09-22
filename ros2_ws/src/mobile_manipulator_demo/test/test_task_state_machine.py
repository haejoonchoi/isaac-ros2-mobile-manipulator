import unittest

from mobile_manipulator_demo.task_state_machine import (
    PickPlaceStateMachine,
    TaskObservation,
    TaskState,
)


class PickPlaceStateMachineTest(unittest.TestCase):
    def test_happy_path_reaches_complete(self):
        machine = PickPlaceStateMachine()

        self.assertEqual(machine.start(), TaskState.NAVIGATING_TO_PICK)
        self.assertEqual(machine.step(TaskObservation(base_at_pick=True)), TaskState.PLANNING_GRASP)
        self.assertEqual(machine.step(TaskObservation(grasp_plan_ready=True)), TaskState.GRASPING)
        self.assertEqual(machine.step(TaskObservation(object_secured=True)), TaskState.LIFTING)
        self.assertEqual(machine.step(TaskObservation(lift_complete=True)), TaskState.NAVIGATING_TO_PLACE)
        self.assertEqual(machine.step(TaskObservation(base_at_place=True)), TaskState.PLACING)
        self.assertEqual(machine.step(TaskObservation(object_released=True)), TaskState.COMPLETE)

    def test_fault_transitions_to_failed(self):
        machine = PickPlaceStateMachine()
        machine.start()

        self.assertEqual(machine.step(TaskObservation(fault="planner timeout")), TaskState.FAILED)

    def test_start_is_only_valid_from_idle(self):
        machine = PickPlaceStateMachine()
        machine.start()

        with self.assertRaises(RuntimeError):
            machine.start()


if __name__ == "__main__":
    unittest.main()

