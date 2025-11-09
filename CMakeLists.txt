# Tetris project
cmake_minimum_required(VERSION 3.10)
project(lecture_17 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the fssimplewindow library from the parent directory
add_subdirectory(../great_YS_libraries/fssimplewindow ${CMAKE_BINARY_DIR}/fssimplewindow_build)
add_subdirectory(../great_YS_libraries/yssimplesound ${CMAKE_BINARY_DIR}/yssimplesound_build)

# Create the executable
add_executable(tetris 
        src/main.cpp
        src/shapes.cpp
        src/grid.cpp
        )


# Link against the fssimplewindow library
target_link_libraries(tetris PRIVATE fssimplewindow)
target_link_libraries(tetris PRIVATE ysglbitmapfont)
target_link_libraries(tetris PRIVATE yssimplesound)
target_link_libraries(tetris PRIVATE asound)