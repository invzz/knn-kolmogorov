set(APP_NAME agnewsloader)

project(${APP_NAME})

include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
)
set (SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c 
         ${PROJECT_SOURCE_DIR}/inc/${APP_NAME}.h
)

set(DEPENDSON models
)
add_library(${PROJECT_NAME} ${SRC})
#target_link_libraries(${PROJECT_NAME} ${DEPENDSON})