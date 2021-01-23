# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "database_wires: 2 messages, 11 services")

set(MSG_I_FLAGS "-Idatabase_wires:/home/kvn/ros/panda_ws/src/database_wires/msg;-Iactionlib_msgs:/opt/ros/melodic/share/actionlib_msgs/cmake/../msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/melodic/share/sensor_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(database_wires_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" "std_msgs/Float64"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" "geometry_msgs/Pose:database_wires/cad_connection:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:database_wires/cad_pins:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:database_wires/cad_pins:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:sensor_msgs/JointState:std_msgs/Header:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_custom_target(_database_wires_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "database_wires" "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" "geometry_msgs/Pose:geometry_msgs/Quaternion:std_msgs/Float64:geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_msg_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)

### Generating Services
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/JointState.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)
_generate_srv_cpp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
)

### Generating Module File
_generate_module_cpp(database_wires
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(database_wires_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(database_wires_generate_messages database_wires_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_cpp _database_wires_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(database_wires_gencpp)
add_dependencies(database_wires_gencpp database_wires_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS database_wires_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_msg_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)

### Generating Services
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/JointState.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)
_generate_srv_eus(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
)

### Generating Module File
_generate_module_eus(database_wires
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(database_wires_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(database_wires_generate_messages database_wires_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_eus _database_wires_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(database_wires_geneus)
add_dependencies(database_wires_geneus database_wires_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS database_wires_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_msg_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)

### Generating Services
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/JointState.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)
_generate_srv_lisp(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
)

### Generating Module File
_generate_module_lisp(database_wires
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(database_wires_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(database_wires_generate_messages database_wires_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_lisp _database_wires_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(database_wires_genlisp)
add_dependencies(database_wires_genlisp database_wires_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS database_wires_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_msg_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)

### Generating Services
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/JointState.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)
_generate_srv_nodejs(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
)

### Generating Module File
_generate_module_nodejs(database_wires
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(database_wires_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(database_wires_generate_messages database_wires_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_nodejs _database_wires_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(database_wires_gennodejs)
add_dependencies(database_wires_gennodejs database_wires_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS database_wires_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_msg_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)

### Generating Services
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/JointState.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)
_generate_srv_py(database_wires
  "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Float64.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
)

### Generating Module File
_generate_module_py(database_wires
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(database_wires_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(database_wires_generate_messages database_wires_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/moveit_parameters.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/pin_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_pose.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/DataSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_info.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/cad_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/StringSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/connection_group.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/IkSrv.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_connection.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/msg/cad_pins.msg" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/database_wires/srv/terminal_data.srv" NAME_WE)
add_dependencies(database_wires_generate_messages_py _database_wires_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(database_wires_genpy)
add_dependencies(database_wires_genpy database_wires_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS database_wires_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/database_wires
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET actionlib_msgs_generate_messages_cpp)
  add_dependencies(database_wires_generate_messages_cpp actionlib_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(database_wires_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(database_wires_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(database_wires_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/database_wires
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET actionlib_msgs_generate_messages_eus)
  add_dependencies(database_wires_generate_messages_eus actionlib_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(database_wires_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(database_wires_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(database_wires_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/database_wires
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET actionlib_msgs_generate_messages_lisp)
  add_dependencies(database_wires_generate_messages_lisp actionlib_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(database_wires_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(database_wires_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(database_wires_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/database_wires
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET actionlib_msgs_generate_messages_nodejs)
  add_dependencies(database_wires_generate_messages_nodejs actionlib_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(database_wires_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(database_wires_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(database_wires_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
    DESTINATION ${genpy_INSTALL_DIR}
    # skip all init files
    PATTERN "__init__.py" EXCLUDE
    PATTERN "__init__.pyc" EXCLUDE
  )
  # install init files which are not in the root folder of the generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires
    DESTINATION ${genpy_INSTALL_DIR}
    FILES_MATCHING
    REGEX "${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/database_wires/.+/__init__.pyc?$"
  )
endif()
if(TARGET actionlib_msgs_generate_messages_py)
  add_dependencies(database_wires_generate_messages_py actionlib_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(database_wires_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(database_wires_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(database_wires_generate_messages_py sensor_msgs_generate_messages_py)
endif()
