# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include;/home/kvn/libfranka/include".split(';') if "${prefix}/include;/home/kvn/libfranka/include" != "" else []
PROJECT_CATKIN_DEPENDS = "moveit_core;moveit_visual_tools;moveit_ros_planning_interface;franka_hw;geometry_msgs;roscpp;rospy;std_msgs;message_runtime;control_msgs;actionlib;actionlib_msgs;sensor_msgs;xmlrpcpp".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lfranka_gripper;/home/kvn/libfranka/build/libfranka.so.0.8.0".split(';') if "-lfranka_gripper;/home/kvn/libfranka/build/libfranka.so.0.8.0" != "" else []
PROJECT_NAME = "gripper_control"
PROJECT_SPACE_DIR = "/home/kvn/ros/panda_ws/install"
PROJECT_VERSION = "0.0.0"
