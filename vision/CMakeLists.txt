cmake_minimum_required(VERSION 3.0.2)
project(vision)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)


## is used, also find other catkin packages

find_package(catkin REQUIRED COMPONENTS
  rospy
  message_generation
  geometry_msgs
  std_msgs
)



add_service_files(
    FILES
   getGraspPose.srv
 )


 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
  geometry_msgs
 )


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  message_generation
  rospy
)
