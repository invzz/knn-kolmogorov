

set(APP_NAME knn)

project(${APP_NAME})
IF (WIN32)
  # set stuff for windows
  find_package(pthread REQUIRED)
  set(DEPENDSON models PThreads4W::PThreads4W)
  ELSE()
  set(DEPENDSON models)
ENDIF()
  
find_package(ZLIB REQUIRED)

set(STATIC_LINK ZLIB::ZLIB models)

set(DEPENDSON ${STATIC_LINK})

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
)

set ( SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c )
add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${APP_NAME} ${DEPENDSON} )


