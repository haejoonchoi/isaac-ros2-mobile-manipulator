FROM ros:humble-ros-base

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-colcon-common-extensions \
    python3-pip \
    ros-humble-moveit \
    ros-humble-tf2-tools \
    ros-humble-rviz2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspaces/isaac-ros2-mobile-manipulator

COPY ros2_ws/src ros2_ws/src

RUN source /opt/ros/humble/setup.bash \
    && cd ros2_ws \
    && colcon build --symlink-install

CMD ["bash"]

