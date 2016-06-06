cmake_minimum_required(VERSION 3.2)
#project(serialcommunication)
include_directories(${serialcommunication_SOURCE_DIR}/external_libs/boost/include)
link_directories(${serialcommunication_SOURCE_DIR}/external_libs/boost/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp SerialClass.cpp SerialClass.h)
add_executable(serialcommunication ${SOURCE_FILES})
target_link_libraries(serialcommunication)