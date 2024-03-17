cmake_minimum_required(VERSION 3.10)

# set the project name
project(lisp.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

# specify where the source files are
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(lispcpp ${SOURCES})

# link the libedit library
target_link_libraries(lispcpp edit)
