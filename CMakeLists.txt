cmake_minimum_required(VERSION 3.15)
project(TerrainGen)

# Find wxWidgets
set(wxWidgets_USE_STATIC ON)
find_package(wxWidgets COMPONENTS core base REQUIRED)

# Find Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)

# Find shaderc
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
find_package(shaderc REQUIRED FATAL_ERROR)

# Configure
set(TG_SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/TGConfig.h.in" ${CMAKE_CURRENT_SOURCE_DIR}/src/TGConfig.h)

# Source
set(TG_SRC
	src/main.cpp
	src/TerrainGenApp.h
	src/TerrainGenApp.cpp
	src/TGConfig.h
	src/TGEvents.h
	src/TGUtils.h
	src/TGUtils.cpp
	src/TGVulkanCanvas.h
	src/TGVulkanCanvas.cpp
	src/TGWindow.h
	src/TGWindow.cpp
)

# Executable
if(WIN32)
	set(TG_EXE_TYPE "WIN32")
endif()

add_executable(terrain-gen ${TG_EXE_TYPE} ${TG_SRC})

# Link libs
include(${wxWidgets_USE_FILE})
target_link_libraries(terrain-gen PUBLIC ${wxWidgets_LIBRARIES})

target_include_directories(terrain-gen PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(terrain-gen PUBLIC ${Vulkan_LIBRARIES})

target_include_directories(terrain-gen PUBLIC ${shaderc_INCLUDE_DIRS})
target_link_libraries(terrain-gen PRIVATE ${shaderc_LIBRARIES})

# GLM (header only)
target_include_directories(terrain-gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glm)

# C++ Standard
set_property(TARGET terrain-gen PROPERTY CXX_STANDARD 17)
