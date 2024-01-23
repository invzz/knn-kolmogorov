cmake_minimum_required(VERSION 3.1...3.28)
project(KNN
        VERSION  0.1
        LANGUAGES C
        )   
include(cmake/init.cmake)
include(cmake/folders.cmake)
include(cmake/targets.cmake)
include(cmake/automate-vcpkg.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_ROOT})



vcpkg_bootstrap()
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist)

