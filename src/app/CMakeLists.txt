cmake_minimum_required(VERSION 3.21)
set(APP_NAME KNN_APP)
project(${APP_NAME})

set(ZLIB_USE_STATIC_LIBS ON)
find_package(ZLIB REQUIRED)
find_package(pthreads REQUIRED)
set(STATIC_LINK PThreads4W::PThreads4W agnewsloader ZLIB::ZLIB models knn)
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

add_executable(${APP_NAME} ${SRC})

target_link_libraries(${APP_NAME} ${DEPENDS_ON} )