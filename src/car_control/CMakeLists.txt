cmake_minimum_required(VERSION 3.0.2)
project(car_control)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  message_generation
  roscpp
  rospy
  std_msgs
  turtlesim
  ecm_bridge
)

find_package(Boost REQUIRED COMPONENTS
  system
  thread
)

add_message_files(
  FILES
  corner.msg
  rover.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES car_control
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
  DEPENDS system_lib Boost
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

add_subdirectory(src/spine)
add_executable(rover_control src/main_draft.cpp)
add_dependencies(rover_control ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(rover_control 
  spine
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  rt
)
catkin_install_python(PROGRAMS
  scripts/joy_listener.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
add_executable(spawn_turtle src/multi_turtle.cpp)
target_link_libraries(spawn_turtle
  ${catkin_LIBRARIES}
  )

add_executable(test_listener src/test_listener.cpp)
target_link_libraries(test_listener
${catkin_LIBRARIES}
)
SET(CMAKE_BUILD_TYPE "RelWithDebInfo")