cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++17) 

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(SOURCE application/familytree/main.cpp "")

project(odb)

add_subdirectory(source)
add_subdirectory(include)
include_directories(include)

add_executable(odb.server ${SOURCE})
target_link_libraries(odb.server c pthread)

