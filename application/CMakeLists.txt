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

add_executable(7-read-imdb.test
  tests/read-file-test.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

add_executable(8-odbd.test
  tests/odbd.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

target_link_libraries(8-odbd.test
  jsoncpp
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(9-odbcli.test
  tests/odbcli.cpp
  $<TARGET_OBJECTS:${PROJECT_NAME}>
  )

target_link_libraries(9-odbcli.test
  ${CMAKE_THREAD_LIBS_INIT}
  )

target_link_libraries(odb.server
  ${CMAKE_THREAD_LIBS_INIT}
  )
