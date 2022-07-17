cmake_minimum_required(VERSION 3.22.2)
project(terrain_gen)

set(TG_SRC
    main.cc
)

add_executable(TG ${TG_SRC})

set_property(TARGET TG PROPERTY CXX_STANDARD 17)