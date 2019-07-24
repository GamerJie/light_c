# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("light")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(libs/sdl2)
add_subdirectory(libs/luajit)

add_definitions(-DFLECS_STATIC)
add_subdirectory(libs/flecs)

# 包含子项目。
add_subdirectory ("light")
