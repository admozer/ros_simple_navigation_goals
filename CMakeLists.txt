cmake_minimum_required(VERSION 2.8.3)
project(simple_navigation_goals)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  move_base_msgs
  nav_msgs
  tf
  roscpp
)

catkin_package()


include_directories(
  ${catkin_INCLUDE_DIRS}
)


add_executable(simple_navigation_goals src/simple_navigation_goals.cpp)
target_link_libraries(simple_navigation_goals ${catkin_LIBRARIES})
