add_executable(odb.server
  familytree/main.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

target_link_libraries(odb.server
  ${CMAKE_THREAD_LIBS_INIT}
  )
