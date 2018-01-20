cmake_minimum_required(VERSION 3.9)
project(odb
  LANGUAGES CXX
  VERSION 0.1.17
  )

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror -pedantic-errors")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

option(BUILD_DOCS "Build the documentation" ON)

find_package(Threads)

include_directories(include)

add_subdirectory(source)
add_subdirectory(include)
add_subdirectory(application)

if(${BUILD_DOCS})
  add_subdirectory(docs)
endif()
