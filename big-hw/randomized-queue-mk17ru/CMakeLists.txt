cmake_minimum_required(VERSION 3.13)

include(test/Strict.cmake)

set(PROJECT_NAME randomized_queue)
project(${PROJECT_NAME})

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inlcude directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Separate executable: main
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Compile source files into a library
add_library(randomized_queue_lib ${SRC_FILES})
target_compile_options(randomized_queue_lib PUBLIC ${COMPILE_OPTS})
target_link_options(randomized_queue_lib PUBLIC ${LINK_OPTS})

# Main is separate
add_executable(subset ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_compile_options(subset PRIVATE ${COMPILE_OPTS})
target_link_options(subset PRIVATE ${LINK_OPTS})

# linking Main against the library
target_link_libraries(subset randomized_queue_lib)

# google test is a git submodule
add_subdirectory(./googletest)

enable_testing()

# Include the gtest library
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# test is a git submodule
add_subdirectory(test)
