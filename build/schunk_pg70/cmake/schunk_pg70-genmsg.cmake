# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "schunk_pg70: 0 messages, 6 services")

set(MSG_I_FLAGS "")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(schunk_pg70_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" ""
)

get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_custom_target(_schunk_pg70_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "schunk_pg70" "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_cpp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
)

### Generating Module File
_generate_module_cpp(schunk_pg70
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(schunk_pg70_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(schunk_pg70_generate_messages schunk_pg70_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_cpp _schunk_pg70_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(schunk_pg70_gencpp)
add_dependencies(schunk_pg70_gencpp schunk_pg70_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS schunk_pg70_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)
_generate_srv_eus(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
)

### Generating Module File
_generate_module_eus(schunk_pg70
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(schunk_pg70_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(schunk_pg70_generate_messages schunk_pg70_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_eus _schunk_pg70_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(schunk_pg70_geneus)
add_dependencies(schunk_pg70_geneus schunk_pg70_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS schunk_pg70_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)
_generate_srv_lisp(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
)

### Generating Module File
_generate_module_lisp(schunk_pg70
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(schunk_pg70_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(schunk_pg70_generate_messages schunk_pg70_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_lisp _schunk_pg70_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(schunk_pg70_genlisp)
add_dependencies(schunk_pg70_genlisp schunk_pg70_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS schunk_pg70_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)
_generate_srv_nodejs(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
)

### Generating Module File
_generate_module_nodejs(schunk_pg70
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(schunk_pg70_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(schunk_pg70_generate_messages schunk_pg70_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_nodejs _schunk_pg70_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(schunk_pg70_gennodejs)
add_dependencies(schunk_pg70_gennodejs schunk_pg70_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS schunk_pg70_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)
_generate_srv_py(schunk_pg70
  "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
)

### Generating Module File
_generate_module_py(schunk_pg70
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(schunk_pg70_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(schunk_pg70_generate_messages schunk_pg70_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/acknowledge_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/reference.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_error.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/get_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/set_position.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kvn/ros/panda_ws/src/schunk_pg70/srv/stop.srv" NAME_WE)
add_dependencies(schunk_pg70_generate_messages_py _schunk_pg70_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(schunk_pg70_genpy)
add_dependencies(schunk_pg70_genpy schunk_pg70_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS schunk_pg70_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/schunk_pg70
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/schunk_pg70
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/schunk_pg70
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/schunk_pg70
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/schunk_pg70
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
