cmake_minimum_required(VERSION 3.20)

project(OpenGLRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(OpenGLRenderer
    src/main.cpp
)

target_include_directories(OpenGLRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

