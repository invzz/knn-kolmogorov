set(APP_NAME models)

project(${APP_NAME})

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src
)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/inc INC)
add_library(${PROJECT_NAME} ${SRC} ${INC})

IF (WIN32)
  # set stuff for windows
  find_package(pthread REQUIRED)
  set(DEPENDSON PThreads4W::PThreads4W)
  target_link_libraries(${APP_NAME} ${DEPENDSON})
ENDIF()

