cmake_minimum_required(VERSION 2.8.3)
project(gripper_control)

add_compile_options(-std=c++11)
set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS
  actionlib  
  actionlib_msgs
  control_msgs
  franka_hw
  geometry_msgs
  message_generation
  moveit_msgs
  moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface
  roscpp
  std_msgs
  sensor_msgs
  xmlrpcpp

)

find_package(Eigen3 REQUIRED)
find_package(Franka 0.5.0 REQUIRED)



add_action_files(
  DIRECTORY action
  FILES Grasp.action
        Homing.action
        Stop.action
        Move.action
)

add_message_files(
  DIRECTORY msg
  FILES GraspEpsilon.msg
        ControlOutput.msg
)


generate_messages(
  DEPENDENCIES
  geometry_msgs
  std_msgs
  actionlib_msgs
  moveit_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES franka_gripper
  CATKIN_DEPENDS 
    moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface

          franka_hw
	  geometry_msgs
	  roscpp 
	  rospy 
	  std_msgs 
	  message_runtime
	  control_msgs
	  actionlib
	  actionlib_msgs
	  sensor_msgs
	  xmlrpcpp

  DEPENDS Franka
)


# Avoid to load multiple time franka_griper that generate an error
if (NOT TARGET franka_gripper)
    add_library(franka_gripper  src/franka_gripper.cpp)
endif() 



include_directories(include)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})
include_directories(${catkin_INCLUDE_DIRS})

if (false)

	# Manual regulated gripper
	add_executable(man_gripper src/manual_gripper_node.cpp)
	target_link_libraries(man_gripper ${catkin_LIBRARIES} ${Boost_LIBRARIES})
	add_dependencies(man_gripper 
	  ${${PROJECT_NAME}_EXPORTED_TARGETS}
	  ${catkin_EXPORTED_TARGETS}
	  franka_gripper)

	add_executable(com_gripper src/GripperUserInterface.cpp)
	target_link_libraries(com_gripper ${catkin_LIBRARIES} ${Boost_LIBRARIES})
	add_dependencies(com_gripper 
	  ${${PROJECT_NAME}_EXPORTED_TARGETS}  
	# Missing this two lines to compile the action correctly
	  ${catkin_EXPORTED_TARGETS}
	  franka_gripper)

	# PID regulated gripper

	add_executable(pid src/PID_gripper_control_node.cpp)
	target_link_libraries(pid ${catkin_LIBRARIES} ${Boost_LIBRARIES})
	add_dependencies(pid 
	  ${${PROJECT_NAME}_EXPORTED_TARGETS}
	  ${catkin_EXPORTED_TARGETS}
	  franka_gripper)
endif()


# Auto Switching gripper-control
add_executable(auto_gripper_control_node src/auto_gripper_control_node.cpp)
target_link_libraries(auto_gripper_control_node ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(auto_gripper_control_node 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 

#Grasp Test
add_executable(TactileSensorGrasp src/TactileSensorGrasp.cpp)
target_link_libraries(TactileSensorGrasp ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(TactileSensorGrasp 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 

add_executable(gripper_controller_schunk src/gripper_controller_schunk.cpp)
target_link_libraries(gripper_controller_schunk ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(gripper_controller_schunk schunk_pg70)




## Tools
include(${CMAKE_CURRENT_LIST_DIR}/../cmake/ClangTools.cmake OPTIONAL
  RESULT_VARIABLE CLANG_TOOLS
)
if(CLANG_TOOLS)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
  file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
  )
  add_format_target(franka_gripper FILES ${SOURCES} ${HEADERS})
  add_tidy_target(franka_gripper
    FILES ${SOURCES}
    DEPENDS franka_gripper franka_gripper_node
  )
endif()





