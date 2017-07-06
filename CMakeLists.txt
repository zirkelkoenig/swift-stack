cmake_minimum_required(VERSION 3.7)
project(tetris)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 2.0 REQUIRED)
find_package(SDL2TTF 2.0 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES
	main.c
	video_data.c
	control.c
	movement.c
	static_data.c
)

add_executable(tetris ${SOURCE_FILES})
target_link_libraries(tetris ${SDL2_LIBRARY})

