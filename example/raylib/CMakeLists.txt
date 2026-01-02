cmake_minimum_required(VERSION 3.22)

project(raylib_test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

add_compile_options("-Wno-enum-conversion")
add_subdirectory(minu)

add_executable(demo main.cpp)

# target_include_directories(demo PUBLIC ${raylib_INCLUDE_DIR})
target_link_libraries(demo PUBLIC minu ${raylib_LIBRARY})
