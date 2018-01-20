add_executable(odb.server
  familytree/main.cpp
  )

target_link_libraries(odb.server
  ${CMAKE_THREAD_LIBS_INIT}
  ${PROJECT_NAME}
  )
