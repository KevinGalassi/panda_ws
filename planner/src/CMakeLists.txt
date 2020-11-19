cmake_minimum_required(VERSION 2.8.3)
project(planner)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  message_generation
  moveit_msgs
  moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface
  roscpp
  std_msgs
  sensor_msgs
)

find_package(Eigen3 REQUIRED)

generate_messages(
  DEPENDENCIES
  actionlib_msgs
  geometry_msgs
  moveit_msgs
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

add_library(TrajectoryPlanner src/TrajectoryPlanner.cpp)
target_link_libraries(TrajectoryPlanner ${catkin_LIBRARIES})


include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})
include_directories(${catkin_INCLUDE_DIRS})
include_directories(moveit_core moveit_visual_tools)







