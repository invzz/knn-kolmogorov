set(JSONPARSERNAME json_parse)
set(APP_NAME data-handler)

project(${APP_NAME})

find_package(json-c CONFIG REQUIRED)

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
    ${LIB_DIR}/category-handler/inc
    
)
set (SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c 
         ${PROJECT_SOURCE_DIR}/inc/${APP_NAME}.h
)

set(DEPENDSON
    json-c
    category-handler
)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${DEPENDSON})