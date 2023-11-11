cmake_minimum_required(VERSION 3.5)

project(terrain-gen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(OpenGL REQUIRED)

set(PROJECT_SOURCES
        FastNoiseLite/FastNoiseLite.h

        bitmap.h
        bitmap.cpp

        camera.h
        camera.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        shader.h
        shader.cpp

        nodes/multiplynoisedatamodel.h
        nodes/multiplynoisedatamodel.cpp
        nodes/noiseoutputdatamodel.h
        nodes/noiseoutputdatamodel.cpp
        nodes/noisesamplerdatamodel.h
        nodes/noisesamplerdatamodel.cpp
        nodes/numbersourcedatamodel.h
        nodes/numbersourcedatamodel.cpp

        nodes/data/integerdata.h
        nodes/data/integerdata.cpp
        nodes/data/noisedata.h
        nodes/data/noisedata.cpp

        nodes/widgets/noisetypeselectorwidget.h
        nodes/widgets/noisetypeselectorwidget.cpp
		
        widgets/heightmapeditortab.h
        widgets/heightmapeditortab.cpp
        widgets/terrainviewwidget.h
        widgets/terrainviewwidget.cpp
)

# various directories we need to keep track of
set(TG_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(QT_NODES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nodeeditor)

# node editor library
add_subdirectory(${QT_NODES_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(terrain-gen
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET terrain-gen APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(terrain-gen SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(terrain-gen
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(terrain-gen PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    QtNodes
    opengl32)
target_include_directories(terrain-gen PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}glm
    ${QT_NODES_DIR})

add_custom_command(
        TARGET terrain-gen POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${TG_SHADER_DIR} $<TARGET_FILE_DIR:terrain-gen>/shaders)

set_target_properties(terrain-gen PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS terrain-gen
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(terrain-gen)
endif()
