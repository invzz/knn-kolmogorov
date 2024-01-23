# getting variables
include(init.cmake)
add_subdirectory(${LIB_DIR}/models)
add_subdirectory(${LIB_DIR}/kd-tree)
add_subdirectory(${KNN_DIR})
add_subdirectory(${APPS_DIR}/KNN-PREDICT)
add_subdirectory(${LIB_DIR}/agnewsloader)