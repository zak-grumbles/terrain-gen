cmake_minimum_required(VERSION 3.22.2)
project(terrain_gen)

# compiler options
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(TG_SRC
    src/main.cc
    src/terrain_gen_app.h
    src/terrain_gen_app.cc
)

set(FastNoise_SRC
    src/FastNoiseLite/FastNoiseLite.h
)

add_executable(TerrainGen ${TG_SRC} ${FastNoise_SRC})

# Ogre
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# Link
target_link_libraries(TerrainGen OgreBites)
