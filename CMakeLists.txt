cmake_minimum_required(VERSION 2.8.3)
project(mocap_csv2bag)

find_package(catkin REQUIRED COMPONENTS
	message_generation
	genmsg
	std_msgs 
	geometry_msgs 
	rosbag
)

add_message_files( 
	DIRECTORY msg
	FILES
	StampedInterruptTime.msg
)

generate_messages( DEPENDENCIES std_msgs geometry_msgs)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS
	genmsg 
    message_runtime
    std_msgs 
	geometry_msgs 
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
	
add_executable(mocap_csv2bag src/csv2bag.cpp)
target_link_libraries(mocap_csv2bag ${catkin_LIBRARIES})
add_dependencies(mocap_csv2bag rosbag ${catkin_EXPORTED_TARGETS})