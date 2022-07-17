cmake_minimum_required(VERSION 3.22.2)
project(terrain_gen)

set(TG_SRC
    src/main.cc
)

add_executable(TerrainGen ${TG_SRC})
set_property(TARGET TerrainGen PROPERTY CXX_STANDARD 17)
