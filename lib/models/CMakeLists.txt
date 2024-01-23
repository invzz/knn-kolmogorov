set(JSONPARSERNAME json_parse)
set(APP_NAME models)

project(${APP_NAME})

find_package(json-c CONFIG REQUIRED)

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/inc
)
set (SRC ${PROJECT_SOURCE_DIR}/inc/models.h )

add_library(${PROJECT_NAME} INTERFACE)
