# Install script for directory: /home/kvn/ros/panda_ws/src/database_wires

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
  include("/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/safe_execute_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires/msg" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
    "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires/srv" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
    "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/installspace/database_wires-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/include/database_wires")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/roseus/ros/database_wires")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/common-lisp/ros/database_wires")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/share/gennodejs/ros/database_wires")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/database_wires")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/database_wires" REGEX "/\\_\\_init\\_\\_\\.py$" EXCLUDE REGEX "/\\_\\_init\\_\\_\\.pyc$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/database_wires" FILES_MATCHING REGEX "/home/kvn/ros/panda_ws/src/build/devel/lib/python2.7/dist-packages/database_wires/.+/__init__.pyc?$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/installspace/database_wires.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires/cmake" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/installspace/database_wires-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires/cmake" TYPE FILE FILES
    "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/installspace/database_wiresConfig.cmake"
    "/home/kvn/ros/panda_ws/src/build/database_wires/catkin_generated/installspace/database_wiresConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/database_wires" TYPE FILE FILES "/home/kvn/ros/panda_ws/src/database_wires/package.xml")
endif()

