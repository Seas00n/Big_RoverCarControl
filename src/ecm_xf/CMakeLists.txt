cmake_minimum_required(VERSION 3.0.2)
project(ecm_xf)

find_package(catkin REQUIRED COMPONENTS
  ecm_bridge
  roscpp
  rospy
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ecm_xf
#  CATKIN_DEPENDS ecm_bridge roscpp rospy
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/ecm_xf_node.cpp)

add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node 
  ${catkin_LIBRARIES}
  rt
)



add_executable(test_node src/test_node.cpp)

add_dependencies(test_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(test_node
  ${catkin_LIBRARIES}
  rt
)
