# getting variables
include(cmake/init.cmake)
add_subdirectory(${LIB_DIR}/models)
add_subdirectory(${KNN_DIR})
add_subdirectory(${APPS_DIR})
add_subdirectory(${LIB_DIR}/agnewsloader)