# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "read_sensor: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iread_sensor:/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(read_sensor_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_custom_target(_read_sensor_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "read_sensor" "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_custom_target(_read_sensor_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "read_sensor" "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" "std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/read_sensor
)
_generate_msg_cpp(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/read_sensor
)

### Generating Services

### Generating Module File
_generate_module_cpp(read_sensor
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/read_sensor
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(read_sensor_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(read_sensor_generate_messages read_sensor_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_cpp _read_sensor_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_cpp _read_sensor_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(read_sensor_gencpp)
add_dependencies(read_sensor_gencpp read_sensor_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS read_sensor_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/read_sensor
)
_generate_msg_eus(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/read_sensor
)

### Generating Services

### Generating Module File
_generate_module_eus(read_sensor
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/read_sensor
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(read_sensor_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(read_sensor_generate_messages read_sensor_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_eus _read_sensor_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_eus _read_sensor_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(read_sensor_geneus)
add_dependencies(read_sensor_geneus read_sensor_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS read_sensor_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/read_sensor
)
_generate_msg_lisp(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/read_sensor
)

### Generating Services

### Generating Module File
_generate_module_lisp(read_sensor
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/read_sensor
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(read_sensor_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(read_sensor_generate_messages read_sensor_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_lisp _read_sensor_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_lisp _read_sensor_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(read_sensor_genlisp)
add_dependencies(read_sensor_genlisp read_sensor_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS read_sensor_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/read_sensor
)
_generate_msg_nodejs(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/read_sensor
)

### Generating Services

### Generating Module File
_generate_module_nodejs(read_sensor
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/read_sensor
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(read_sensor_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(read_sensor_generate_messages read_sensor_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_nodejs _read_sensor_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_nodejs _read_sensor_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(read_sensor_gennodejs)
add_dependencies(read_sensor_gennodejs read_sensor_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS read_sensor_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor
)
_generate_msg_py(read_sensor
  "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor
)

### Generating Services

### Generating Module File
_generate_module_py(read_sensor
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(read_sensor_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(read_sensor_generate_messages read_sensor_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/proximity_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_py _read_sensor_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/tactile_sensor/read_sensor/msg/tactile_sensor_data.msg" NAME_WE)
add_dependencies(read_sensor_generate_messages_py _read_sensor_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(read_sensor_genpy)
add_dependencies(read_sensor_genpy read_sensor_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS read_sensor_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/read_sensor)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/read_sensor
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(read_sensor_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/read_sensor)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/read_sensor
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(read_sensor_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/read_sensor)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/read_sensor
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(read_sensor_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/read_sensor)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/read_sensor
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(read_sensor_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/read_sensor
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(read_sensor_generate_messages_py std_msgs_generate_messages_py)
endif()
