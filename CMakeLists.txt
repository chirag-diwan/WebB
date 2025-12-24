cmake_minimum_required(VERSION 3.20)

project(WebB VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WebB)

target_sources(WebB PRIVATE
  src/main.cpp
  src/display/display.cpp
  src/interpreter/interpreter.cpp
  src/parser/parser.cpp
)

target_include_directories(WebB PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(raylib REQUIRED)
target_link_libraries(WebB PRIVATE raylib)
