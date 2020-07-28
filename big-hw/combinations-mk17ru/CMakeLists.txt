cmake_minimum_required(VERSION 3.13)

include(test/Strict.cmake)

set(PROJECT_NAME combinations)
project(${PROJECT_NAME})

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Filesystem REQUIRED)

# XML parsing subproject
add_subdirectory(xml)
add_library(xml_lib ALIAS pugixml)

# Inlcude directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Separate executable: main
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Compile source files into a library
add_library(combinations_lib ${SRC_FILES})
target_link_libraries(combinations_lib PUBLIC xml_lib std::filesystem)

# Main is separate
add_executable(combinations ${PROJECT_SOURCE_DIR}/src/main.cpp)

# linking Main against the library
target_link_libraries(combinations combinations_lib)

# google test is a git submodule
add_subdirectory(googletest)

enable_testing()

# test is a git submodule
add_subdirectory(test)

add_test(NAME tests COMMAND runUnitTests)
