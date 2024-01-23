cmake_minimum_required(VERSION 3.21)
set(LIB_NAME KNN_APP)
project(${LIB_NAME})

set(ZLIB_USE_STATIC_LIBS ON)
find_package(ZLIB REQUIRED)

set(STATIC_LINK  pthread agnewsloader z models knn)
set(DEPENDS_ON ${STATIC_LINK})
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

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${DEPENDS_ON} )