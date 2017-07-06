cmake_minimum_required(VERSION 3.7)
project(tetris)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 2.0 REQUIRED)
find_package(SDL2TTF 2.0 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2TTF_INCLUDE_DIR})

set(SOURCE_FILES
	main.c
	video_data.c
	control.c
	movement.c
	static_data.c
)

add_executable(tetris ${SOURCE_FILES})
target_link_libraries(tetris ${SDL2_LIBRARY})
target_link_libraries(tetris ${SDL2TTF_LIBRARY})

if(WIN32)
	set_target_properties(tetris PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
	set_target_properties(tetris PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
	set_target_properties(tetris PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
	set_target_properties(tetris PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
	set_target_properties(tetris PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(tetris PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)
