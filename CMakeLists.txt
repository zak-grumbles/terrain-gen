cmake_minimum_required(VERSION 3.22.2)
project(terrain_gen)

# compiler options
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")

set(TG_SRC
    src/main.cc
)

add_executable(TerrainGen ${TG_SRC})

# Ogre
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# Link
target_link_libraries(TerrainGen OgreBites)
