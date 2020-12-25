cmake_minimum_required(VERSION 2.8.3)
project(read_sensor)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  serial
  message_generation
)

add_library( TactileSensor
  src/TactileSensor.cpp
)



add_message_files(
  FILES
  proximity_sensor_data.msg
  tactile_sensor_data.msg
)


generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package()


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_dependencies(TactileSensor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(${PROJECT_NAME}_node src/read_sensor.cpp)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} serial)

add_executable(tactile_data_read src/tactile_data_read.cpp)
add_dependencies(tactile_data_read ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(tactile_data_read ${catkin_LIBRARIES} ${Boost_LIBRARIES})





