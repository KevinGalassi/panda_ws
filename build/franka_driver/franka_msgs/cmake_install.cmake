# Install script for directory: /home/kvn/ros/panda_ws/src/franka_driver/franka_msgs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/msg/Errors.msg"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/msg/FrankaState.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/srv" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetCartesianImpedance.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetEEFrame.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetForceTorqueCollisionBehavior.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetFullCollisionBehavior.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetJointImpedance.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetKFrame.srv"
    "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/srv/SetLoad.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/action" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/action/ErrorRecovery.action")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryAction.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryActionGoal.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryActionResult.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryActionFeedback.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryGoal.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryResult.msg"
    "/home/kvn/ros/panda_ws/src/build/devel/share/franka_msgs/msg/ErrorRecoveryFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/franka_driver/franka_msgs/catkin_generated/installspace/franka_msgs-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/include/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/roseus/ros/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/common-lisp/ros/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/gennodejs/ros/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/franka_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/franka_driver/franka_msgs/catkin_generated/installspace/franka_msgs.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/franka_driver/franka_msgs/catkin_generated/installspace/franka_msgs-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs/cmake" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/build/franka_driver/franka_msgs/catkin_generated/installspace/franka_msgsConfig.cmake"
    "/home/kvn/ros/panda_ws/src/build/franka_driver/franka_msgs/catkin_generated/installspace/franka_msgsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/franka_msgs" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/franka_driver/franka_msgs/package.xml")
endif()

