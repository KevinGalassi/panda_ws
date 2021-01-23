execute_process(COMMAND "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
