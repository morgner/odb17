add_executable(odb.server
  familytree/main.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(1-atom.test
  tests/atom-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(2-atom2thing.test
  tests/atom2thing-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(3-property.test
  tests/property2thing-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(4-link2things.test
  tests/link2things-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(5-things.test
  tests/things-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(6-json-read.test
  tests/json-read-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

target_link_libraries(6-json-read.test
  jsoncpp
  )

target_link_libraries(odb.server
  ${CMAKE_THREAD_LIBS_INIT}
  )
