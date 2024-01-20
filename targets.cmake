# getting variables
include(init.cmake)
add_subdirectory(${LIB_DIR}/models)
add_subdirectory(${LIB_DIR}/category-handler)
add_subdirectory(${LIB_DIR}/data-handler)
add_subdirectory(${KNN_DIR})
add_subdirectory(${APPS_DIR})