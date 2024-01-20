set(APP_NAME category-handler)

project(${APP_NAME})

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
)
set (SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c 
         ${PROJECT_SOURCE_DIR}/inc/${APP_NAME}.h
)

set(DEPENDSON
)
add_library(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${DEPENDSON})