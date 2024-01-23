set(JSONPARSERNAME json_parse)
set(APP_NAME kd-handler)

project(${APP_NAME})

find_package(json-c CONFIG REQUIRED)

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
  
    
)
set (SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c 
         ${PROJECT_SOURCE_DIR}/inc/${APP_NAME}.h
)

set(DEPENDSON
    json-c
)

add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${DEPENDSON})