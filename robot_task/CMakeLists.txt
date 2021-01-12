cmake_minimum_required(VERSION 2.8.3)
project(robot_task)

add_compile_options(-std=c++11)
set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  franka_gripper
  message_generation
  moveit_msgs
  moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface
  roscpp
  std_msgs
  sensor_msgs
  vision
)

find_package(Eigen3 REQUIRED)
find_package(Franka 0.5.0 REQUIRED)



catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS 
  LIBRARIES franka_gripper
  actionlib
  actionlib_msgs
  geometry_msgs
  franka_gripper
  message_runtime
  moveit_msgs 
  moveit_core
  moveit_visual_tools
  moveit_ros_planning_interface
  roscpp 
  rospy 
  sensor_msgs
  std_msgs 

  DEPENDS Franka
)

if (NOT TARGET franka_gripper)
    add_library(franka_gripper  src/franka_gripper.cpp)
endif() 



include_directories(
	include
	franka_gripper
	SYSTEM ${EIGEN3_INCLUDE_DIRS}
	${catkin_INCLUDE_DIRS}
	moveit_core 
	moveit_visual_tools
  vision
)



add_executable(cable_routing src/cable_routing.cpp)
target_link_libraries(cable_routing ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(cable_routing 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 



add_executable(active_control src/active_control)
target_link_libraries(active_control ${catkin_LIBRARIES} ${Boost_LIBRARIES})


add_executable(static_grasp src/static_grasp.cpp)
target_link_libraries(static_grasp ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(static_grasp 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  vision) 


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







