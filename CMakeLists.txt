cmake_minimum_required(VERSION 3.0)
project(ultraLiteDatabase)

set(CMAKE_CXX_STANDARD 26)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(ultraLiteDatabase main.cpp
        imgui_backends/imgui_impl_glfw.cpp
        imgui_backends/imgui_impl_opengl3.cpp)

target_link_libraries(ultraLiteDatabase PRIVATE imgui::imgui glfw OpenGL::GL)
