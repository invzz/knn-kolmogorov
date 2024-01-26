

set(APP_NAME knn)

project(${APP_NAME})
find_package(pthread REQUIRED)
find_package(ZLIB REQUIRED)
set(STATIC_LINK ZLIB::ZLIB models)
set(DEPENDS_ON ${STATIC_LINK})
include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
)
set ( SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c )
add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${APP_NAME} ${DEPENDS_ON} )


