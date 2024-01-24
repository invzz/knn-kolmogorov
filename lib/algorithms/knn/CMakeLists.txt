

set(APP_NAME knn)

project(${APP_NAME})
set(STATIC_LINK data-handler z models)
set(DEPENDS_ON ${STATIC_LINK})
include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
)
set ( SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c )

add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} models)