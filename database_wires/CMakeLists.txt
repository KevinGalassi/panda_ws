cmake_minimum_required(VERSION 2.8.3)
project(database_wires)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib_msgs
  message_generation
  roscpp
  cv_bridge
  rospy
  std_msgs
  geometry_msgs
  kdl_parser
  kdl_conversions
  dynamic_reconfigure
  sensor_msgs
)

find_package( Eigen3 REQUIRED )
catkin_python_setup()

find_package(OpenCV)

include_directories(
  include ${catkin_INCLUDE_DIRS} /usr/include/eigen3 ${orocos_kdl_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})


add_message_files(FILES
  cad_connection.msg
  cad_pins.msg
)

add_service_files(FILES
  moveit_parameters.srv
  terminal_data.srv
  connection_data.srv
  connection_group.srv
  pin_data.srv
  IkSrv.srv
  DataSrv.srv
  StringSrv.srv
  cad_data.srv
  cad_pose.srv
  cad_info.srv
)


generate_messages(DEPENDENCIES
  actionlib_msgs
  std_msgs
  geometry_msgs
  sensor_msgs
)

catkin_package(
  CATKIN_DEPENDS
  message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

