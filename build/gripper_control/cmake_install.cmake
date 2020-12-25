# Install script for directory: /home/kvn/ros/panda_ws/src/gripper_control

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/kvn/ros/panda_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/action" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/gripper_control/action/Grasp.action"
    "/home/kvn/ros/panda_ws/src/gripper_control/action/Homing.action"
    "/home/kvn/ros/panda_ws/src/gripper_control/action/Stop.action"
    "/home/kvn/ros/panda_ws/src/gripper_control/action/Move.action"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspAction.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspActionGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspActionResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspActionFeedback.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/GraspFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingAction.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingActionGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingActionResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingActionFeedback.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/HomingFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopAction.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopActionGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopActionResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopActionFeedback.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/StopFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveAction.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveActionGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveActionResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveActionFeedback.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveGoal.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveResult.msg"
    "/home/kvn/ros/panda_ws/devel/share/gripper_control/msg/MoveFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/gripper_control/msg/GraspEpsilon.msg"
    "/home/kvn/ros/panda_ws/src/gripper_control/msg/ControlOutput.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/build/gripper_control/catkin_generated/installspace/gripper_control-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/devel/include/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/devel/share/roseus/ros/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/devel/share/common-lisp/ros/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/devel/share/gennodejs/ros/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/kvn/ros/panda_ws/devel/lib/python2.7/dist-packages/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/devel/lib/python2.7/dist-packages/gripper_control")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/kvn/ros/panda_ws/build/gripper_control/catkin_generated/installspace/gripper_control.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/build/gripper_control/catkin_generated/installspace/gripper_control-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control/cmake" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/build/gripper_control/catkin_generated/installspace/gripper_controlConfig.cmake"
    "/home/kvn/ros/panda_ws/build/gripper_control/catkin_generated/installspace/gripper_controlConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gripper_control" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/gripper_control/package.xml")
endif()

