cmake_minimum_required(VERSION 3.20)

project(OpenGLRenderer LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glfw)
if(MSVC)
    set_property(TARGET glfw PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core_43 REPRODUCIBLE API gl:core=4.3)
if(MSVC)
    set_property(TARGET glad_gl_core_43 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glm)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.4.3
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(assimp)
if(MSVC)
    set_property(TARGET assimp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET zlibstatic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.9b
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_compile_definitions(imgui PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_GLAD2
)

target_link_libraries(imgui PUBLIC
    glad_gl_core_43
    glfw
    OpenGL::GL
)

if(MSVC)
    set_property(TARGET imgui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_executable(OpenGLRenderer
    src/main.cpp
    src/Core/Application.h
    src/Core/Application.cpp
    src/Core/ImGuiLayer.h
    src/Core/ImGuiLayer.cpp
    src/Core/Window.h
    src/Core/Window.cpp
    src/Renderer/Shader.h
    src/Renderer/Shader.cpp
    src/Renderer/Buffer.h
    src/Renderer/Buffer.cpp
    src/Renderer/VertexArray.h
    src/Renderer/VertexArray.cpp
    src/Renderer/Mesh.h
    src/Renderer/Mesh.cpp
    src/Renderer/Model.h
    src/Renderer/Model.cpp
    src/Renderer/Material.h
    src/Renderer/Material.cpp
    src/Scene/Transform.h
    src/Scene/Transform.cpp
    src/Scene/Camera.h
    src/Scene/Camera.cpp
    src/Scene/RenderComponent.h
    src/Scene/RenderComponent.cpp
    src/Scene/GameObject.h
    src/Scene/GameObject.cpp
    src/Scene/Scene.h
    src/Scene/Scene.cpp
    src/Scene/CameraController.h
    src/Scene/CameraController.cpp
)

target_include_directories(OpenGLRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(OpenGLRenderer PRIVATE
    glad_gl_core_43
    assimp
    glfw
    glm::glm
    imgui
    OpenGL::GL
)

if(MSVC)
    target_compile_options(OpenGLRenderer PRIVATE /utf-8)
endif()
