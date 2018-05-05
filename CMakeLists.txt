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

option(BUILD_DOCS "Build the documentation" OFF)

find_package(Threads)

include_directories(include)

add_subdirectory(source)
add_subdirectory(include)
add_subdirectory(application)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      OFF)
find_package(Boost 1.64.0 REQUIRED COMPONENTS container)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
#  add_executable(foo foo.cc)
#  target_link_libraries(foo ${Boost_LIBRARIES})
endif()

if(${BUILD_DOCS})
  add_subdirectory(docs)
endif()
