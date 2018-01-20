add_custom_target(docs ALL
  make html
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Building the ${PROJECT_NAME} documentation"
  )
