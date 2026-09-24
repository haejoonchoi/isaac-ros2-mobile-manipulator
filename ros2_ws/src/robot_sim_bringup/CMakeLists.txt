cmake_minimum_required(VERSION 3.16)
project(robot_sim_bringup)

find_package(ament_cmake REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  DIRECTORY config launch maps urdf worlds
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
