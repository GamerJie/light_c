# CMakeList.txt: light 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
project(light)

include_directories(../libs/jsmn)
include_directories(../libs/luajit/src)
include_directories(../libs/sdl2/include)
include_directories(../libs/flecs/include)

include_directories(../libs/admin/include)
include_directories(../libs/civetweb/include)

include_directories(include)

file(GLOB sources sources/*.c)
file(GLOB admin ../libs/admin/src/*.c)
file(GLOB civetweb ../libs/civetweb/src/*.c)

# 将源代码添加到此项目的可执行文件。
add_executable (light ${sources})

# TODO: 如有需要，请添加测试并安装目标。

target_link_libraries (light liblua SDL2-static flecs_static)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)