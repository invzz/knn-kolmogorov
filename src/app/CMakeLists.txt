cmake_minimum_required(VERSION 3.21)
set(APP_NAME KNN_APP)
project(${APP_NAME})

set(ZLIB_USE_STATIC_LIBS ON)
IF (WIN32)
  # set stuff for windows
  find_package(pthread REQUIRED)
  set(DEPENDSON PThreads4W::PThreads4W agnewsloader ZLIB::ZLIB models knn)
ELSE()
  set(DEPENDSON agnewsloader ZLIB::ZLIB models knn)
ENDIF()

find_package(ZLIB REQUIRED)


# includes search paths
include_directories(
    BEFORE
    ${ZLIB_INCLUDE_DIRS}
    ${RES_DIR}
    ${LIB_DIR}/models/inc
    ${LIB_DIR}/agnewsloader/inc
    ${KNN_DIR}/knn/inc
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

add_executable(${APP_NAME} ${SRC})

target_link_libraries(${APP_NAME} ${DEPENDSON} )