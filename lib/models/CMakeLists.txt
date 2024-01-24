set(JSONPARSERNAME json_parse)
set(APP_NAME models)

project(${APP_NAME})

find_package(json-c CONFIG REQUIRED)

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src
)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/inc INC)

add_library(${PROJECT_NAME} ${SRC} ${INC})

