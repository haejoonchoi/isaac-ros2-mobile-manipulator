from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription(
        [
            Node(
                package="mobile_manipulator_demo",
                executable="task_state_machine",
                name="task_state_machine",
                output="screen",
            )
        ]
    )

