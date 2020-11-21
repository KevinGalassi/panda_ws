cmake_minimum_required(VERSION 2.8.3)
project(test)

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
)



add_executable(te src/test.cpp)
target_link_libraries(te ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(te 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 


add_executable(force_test src/force_test.cpp)
target_link_libraries(force_test ${catkin_LIBRARIES} ${Boost_LIBRARIES})


add_executable(test_correlazione_sensore src/test_correlazione_sensore.cpp)
target_link_libraries(test_correlazione_sensore ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(test_correlazione_sensore 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 

add_executable(velocity_test src/velocity_test)
target_link_libraries(velocity_test ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(active_control src/active_control)
target_link_libraries(active_control ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(Prove_tattili src/Prove_tattili)
target_link_libraries(Prove_tattili ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(Prove_tattili 
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  franka_gripper) 


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







