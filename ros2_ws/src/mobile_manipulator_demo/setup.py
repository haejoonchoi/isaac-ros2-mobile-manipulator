from setuptools import find_packages, setup

package_name = "mobile_manipulator_demo"

setup(
    name=package_name,
    version="0.1.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", [f"resource/{package_name}"]),
        (f"share/{package_name}", ["package.xml"]),
        (f"share/{package_name}/launch", ["launch/demo.launch.py"]),
        (f"share/{package_name}/config", ["config/task.yaml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Your Name",
    maintainer_email="you@example.com",
    description="Isaac Sim and ROS2 mobile manipulator pick-and-place demo.",
    license="MIT",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "task_state_machine = mobile_manipulator_demo.task_state_machine:main",
        ],
    },
)

