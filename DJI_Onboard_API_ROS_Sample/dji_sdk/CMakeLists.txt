cmake_minimum_required(VERSION 2.8.3)
project(dji_sdk)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  nav_msgs
  geometry_msgs
   message_generation
)

find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   attitude_quad.msg
   global_position.msg
   velocity.msg
   acc.msg
   gimbal.msg
   local_position.msg
   rc_channels.msg
 )

## Generate services in the 'srv' folder
 add_service_files(
   FILES
   test_basic_command.srv
   test_movement_control.srv
   test_gimbal_control.srv
   test_simple_task.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES dji_sdk
  CATKIN_DEPENDS roscpp std_msgs rospy nav_msgs geometry_msgs
  DEPENDS system_lib
)
 

file(GLOB DJI_SDK_LIB_SOURCES src/sdk_lib/*.cpp) 


include_directories(
  ${catkin_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/src/"
  "${PROJECT_SOURCE_DIR}/include"
  ../../devel/include
  ex_include/
)

## Declare a cpp executable
add_executable(dji_sdk_node src/dji_sdk_node.cpp
    src/modules/dji_gimbal.cpp
    src/modules/dji_services.cpp
    src/modules/dji_variable.cpp
    src/modules/dji_subscribers.cpp
    ${DJI_SDK_LIB_SOURCES}
  src/modules/dji_services.cpp
  include/dji_sdk/dji_services.h
  src/modules/dji_publishers.cpp
   include/dji_sdk/dji_publishers.h
   include/dji_sdk/dji_subscribers.h
 src/modules/motion_controls.cpp
 src/modules/dji_commands.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
add_dependencies(dji_sdk_node dji_sdk_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(dji_sdk_node
   ${catkin_LIBRARIES}
)
add_executable(sdk_client src/sdk_client.cpp)
target_link_libraries(sdk_client ${catkin_LIBRARIES})
 add_dependencies(sdk_client dji_sdk_generate_messages_cpp)
