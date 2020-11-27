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



add_library(
  Planner src/TrajectoryPlanner.cpp
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

add_dependencies(Planner ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


add_executable(test_planner src/test.cpp)
target_link_libraries(test_planner Planner ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(test_planner Planner
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 



add_executable(test_tactile src/TactileSensorTesting.cpp)
target_link_libraries(test_tactile Planner ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(test_tactile Planner
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 




