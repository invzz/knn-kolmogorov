set(APP_NAME agnewsloader)

project(${APP_NAME})
find_package(pthread REQUIRED)
include_directories(
    BEFORE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc
    ${LIB_DIR}/models/inc
)
set (SRC ${PROJECT_SOURCE_DIR}/src/${APP_NAME}.c 
         ${PROJECT_SOURCE_DIR}/inc/${APP_NAME}.h
)

set(DEPENDSON models PThreads4W::PThreads4W
)
add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${DEPENDSON})