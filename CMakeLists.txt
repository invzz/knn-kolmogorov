cmake_minimum_required(VERSION 3.1...3.28)

project(KNN
        VERSION  0.1
        LANGUAGES C
        )   

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist)

include(init.cmake)
include(folders.cmake)
include(targets.cmake)
