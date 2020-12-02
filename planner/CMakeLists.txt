cmake_minimum_required(VERSION 2.8.3)
project(planner)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  message_generation
  message_runtime
  moveit_msgs
  moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface
  roscpp
  rospy
  std_msgs
  sensor_msgs
)

find_package(Eigen3 REQUIRED)



add_library(
  TrajectoryPlanner src/TrajectoryPlanner.cpp
)


add_action_files(
  DIRECTORY action
  FILES
  ComputeTrajectory.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs
               geometry_msgs
               std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS 
    actionlib
    actionlib_msgs
    geometry_msgs
    message_runtime
    moveit_msgs 
    moveit_core
    moveit_visual_tools
    moveit_ros_planning_interface
    roscpp 
    rospy 
    sensor_msgs
    std_msgs 
)


include_directories(
  include
  SYSTEM ${EIGEN3_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  moveit_core
  moeit_visual_tools
)

add_dependencies(TrajectoryPlanner ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}  ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )


add_executable(test_planner src/test.cpp)
target_link_libraries(test_planner TrajectoryPlanner ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(test_planner TrajectoryPlanner
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 


add_executable(planning_action_server src/planning_action_server.cpp)
target_link_libraries(planning_action_server ${catkin_LIBRARIES} TrajectoryPlanner)
add_dependencies(planning_action_server TrajectoryPlanner 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  )

add_executable(test_tactile src/TactileSensorTesting.cpp)
target_link_libraries(test_tactile Planner ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(test_tactile Planner
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 




