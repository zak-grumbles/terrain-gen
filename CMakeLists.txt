cmake_minimum_required(VERSION 3.22.2)
project(terrain_gen)

include(CMakePrintHelpers)

# compiler options
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(TG_SRC
    src/generator.h
    src/generator.cc
    src/main.cc
    src/terrain_gen_app.h
    src/terrain_gen_app.cc
    src/types.h
    src/tables.h
)

set(FastNoise_SRC
    src/FastNoiseLite/FastNoiseLite.h
)

add_executable(TerrainGen ${TG_SRC} ${FastNoise_SRC})

# Ogre
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# info
cmake_print_variables(OGRE_LIBRARY_DIRS)
cmake_print_variables(OGRE_LIBRARIES)

# Link
target_link_libraries(TerrainGen OgreBites)
target_include_directories(TerrainGen PUBLIC ${OGRE_INCLUDE_DIRS}/..)

if(WIN32)
	add_custom_command(TARGET TerrainGen POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_DIR}/../bin/ $<TARGET_FILE_DIR:TerrainGen>)
endif(WIN32)
