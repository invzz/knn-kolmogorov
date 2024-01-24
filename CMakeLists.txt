cmake_minimum_required(VERSION 3.1...3.28)
project(KNN
        VERSION  0.1
        LANGUAGES C
        )   
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)
include(cmake/init.cmake)
include(cmake/folders.cmake)
message(STATUS "RES_DIR: " ${PROJECT_SOURCE_DIR}/resources/)
add_definitions( -DTRAIN_PATH="${PROJECT_SOURCE_DIR}/resources/train.csv" )
add_definitions( -DTEST_PATH="${PROJECT_SOURCE_DIR}/resources/test.csv" )
include(cmake/targets.cmake)
include(cmake/automate-vcpkg.cmake)
vcpkg_bootstrap()

