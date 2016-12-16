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
file(GLOB_RECURSE AT_XIB_FILES ${TOP_DIR} ${TOP_DIR}/resources/*.xib)

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

# source_group("core\\include" FILES ${CORE_OSX_INCLUDE_FILES})
# source_group("core\\include" FILES ${CORE_OSX_OBJC_INCLUDE_FILES})

# Widget objects library.
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${AT_SOURCE_FILES} ${AT_INCLUDE_FILES} ${AT_INCLUDE_H_FILES} ${AT_XIB_FILES})

# Target properties.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_CXX_FLAGS "-fPIC -std=c++14 ${CMAKE_CXX_FLAGS}")

set_source_files_properties(${AT_XIB_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.ax.AudioTool")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version 0.1")
set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING 0.1)
set(MACOSX_BUNDLE_BUNDLE_VERSION 0.1)
set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2016. All Rights Reserved.")

# These variables are specific to our plist and are NOT standard CMake variables
set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")

# set_target_properties(Widget PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/plist.in)
# Set a custom plist file for the app bundle
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${TOP_DIR}/resources/Info.plist)
# set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER AudioTools/)

# Include directory.
target_include_directories(${PROJECT_NAME} PUBLIC ${TOP_DIR}/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /opt/local/include/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/freetype2/)
target_include_directories(${PROJECT_NAME} PUBLIC /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/)
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/lib/libzip/include)

# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/core/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/core/osx)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/util/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/event/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/os/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/attribute/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/graphic/include)
# target_include_directories(Widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/widget/include)
# target_include_directories(Widget PUBLIC ${FREETYPE2_INCLUDE_DIRS})
# target_include_directories(Widget PUBLIC ${PNG_INCLUDE_DIRS})

link_directories(/usr/local/lib/)
# target_link_libraries(${PROJECT_NAME} /usr/local/lib/libaxlib.dylib)
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
# target_link_libraries(${PROJECT_NAME} /usr/local/lib/libboost_chrono.a)

# /usr/local/lib/libaxAudio.a 
# /usr/local/lib/libaxMidi.a 
# /usr/local/lib/libportaudio.a 
# /usr/local/lib/libfreetype.a 
# /usr/local/lib/libsndfile.a 
# /usr/local/lib/libpng16.a 
# /usr/local/lib/libOpenAX.a 
# /usr/local/lib/libboost_python.a 
# /usr/local/lib/libboost_filesystem.a 
# /usr/local/lib/libboost_system.a 
# /usr/local/lib/libboost_chrono.a 
# /usr/local/lib/libboost_thread.a 
# /usr/local/lib/libboost_random.a 
# /usr/local/lib/libzip.a /usr/local/opt/openssl/lib/libcrypto.a /usr/local/opt/openssl/lib/libssl.a
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libOpenAX.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libaxMidi.a)
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libaxAudio.a)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Python")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreAudio")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreMIDI")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AudioUnit")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreAudioKit")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreFoundation")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AudioToolbox")
