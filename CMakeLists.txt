cmake_minimum_required(VERSION 3.2.1)

# Prevent from running cmake in root folder.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "cmake needs to be called in a sub directory.")
endif ()

project(AudioTools)

# include(../../cmake/FindFreetype2.cmake)
# include(../../cmake/FindPng.cmake)

# Use folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# AudioTools files.
file(GLOB_RECURSE AT_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/*.hpp)
file(GLOB_RECURSE AT_INCLUDE_H_FILES ${TOP_DIR} ${TOP_DIR}/include/*.h)
file(GLOB_RECURSE AT_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/*.cpp)
# file(GLOB_RECURSE AT_XIB_FILES ${TOP_DIR} ${TOP_DIR}/resources/*.xib)

# Atk.
file(GLOB ATK_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/atk/*.hpp)
file(GLOB ATK_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/atk/*.cpp)
source_group("include\\atk" FILES ${ATK_INCLUDE_FILES})
source_group("source\\atk" FILES ${ATK_SOURCE_FILES})

# Python.
file(GLOB AT_PYTHON_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/python/*.hpp)
file(GLOB AT_PYTHON_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/python/*.cpp)
source_group("include\\python" FILES ${AT_PYTHON_INCLUDE_FILES})
source_group("source\\pyhton" FILES ${AT_PYTHON_SOURCE_FILES})

# Editor.
file(GLOB AT_EDITOR_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/editor/*.hpp)
file(GLOB AT_EDITOR_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/editor/*.cpp)
source_group("include\\editor" FILES ${AT_EDITOR_INCLUDE_FILES})
source_group("source\\editor" FILES ${AT_EDITOR_SOURCE_FILES})

# Project
file(GLOB AT_PROJECT_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/project/*.hpp)
file(GLOB AT_PROJECT_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/project/*.cpp)
source_group("include\\project" FILES ${AT_PROJECT_INCLUDE_FILES})
source_group("source\\project" FILES ${AT_PROJECT_SOURCE_FILES})

# Dialog
file(GLOB AT_DIALOG_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/dialog/*.hpp)
file(GLOB AT_DIALOG_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/dialog/*.cpp)
source_group("include\\dialog" FILES ${AT_DIALOG_INCLUDE_FILES})
source_group("source\\dialog" FILES ${AT_DIALOG_SOURCE_FILES})

# Widget
file(GLOB AT_WIDGET_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/widget/*.hpp)
file(GLOB AT_WIDGET_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/widget/*.cpp)
source_group("include\\widget" FILES ${AT_WIDGET_INCLUDE_FILES})
source_group("source\\widget" FILES ${AT_WIDGET_SOURCE_FILES})

# Menu attribute.
file(GLOB AT_M_ATT_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/menu/attribute/*.hpp)
file(GLOB AT_M_ATT_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/menu/attribute/*.cpp)
source_group("include\\menu\\attribute" FILES ${AT_M_ATT_INCLUDE_FILES})
source_group("source\\menu\\attribute" FILES ${AT_M_ATT_SOURCE_FILES})

# Menu widget.
file(GLOB AT_M_WIDGET_INCLUDE_FILES ${TOP_DIR} ${TOP_DIR}/include/menu/widget/*.hpp)
file(GLOB AT_M_WIDGET_SOURCE_FILES ${TOP_DIR} ${TOP_DIR}/source/menu/widget/*.cpp)
source_group("include\\menu\\attrwidgetibute" FILES ${AT_M_WIDGET_INCLUDE_FILES})
source_group("source\\menu\\widget" FILES ${AT_M_WIDGET_SOURCE_FILES})

# App icon path.
set(APP_ICON ${TOP_DIR}/resources/at_icon.icns)

# Audiotool application.
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${AT_SOURCE_FILES} ${AT_INCLUDE_FILES} ${AT_INCLUDE_H_FILES} ${APP_ICON})

# Target properties.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_CXX_FLAGS "-fPIC -std=c++14 ${CMAKE_CXX_FLAGS}")

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.ax.AudioTool")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version 0.3")
set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING 0.1)
set(MACOSX_BUNDLE_BUNDLE_VERSION 0.1)
set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2016. All Rights Reserved.")

# These variables are specific to our plist and are NOT standard CMake variables
set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")
set(MACOSX_BUNDLE_ICON_FILE at_icon)

set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

# set_target_properties(Widget PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/plist.in)
# Set a custom plist file for the app bundle
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${TOP_DIR}/resources/Info.plist)

# Include directory.
target_include_directories(${PROJECT_NAME} PUBLIC ${TOP_DIR}/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /opt/local/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/freetype2/)
target_include_directories(${PROJECT_NAME} PUBLIC /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/lib/libzip/include)

link_directories(/usr/local/lib/)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libsndfile.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libfreetype.dylib)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libpng16.dylib)
target_link_libraries(${PROJECT_NAME} /usr/lib/libz.dylib)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libzip.dylib)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libssl.dylib)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libcrypto.dylib)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libportaudio.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_system.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_python.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_filesystem.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_chrono.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_thread.a )
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_random.a)

target_link_libraries(${PROJECT_NAME} /usr/local/lib/libaxlib.a)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Python")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreAudio")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreMIDI")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AudioUnit")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreAudioKit")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreFoundation")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AudioToolbox")
