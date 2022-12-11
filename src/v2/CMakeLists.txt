cmake_minimum_required(VERSION 3.9)

project(capitalism_simulator)
set (CMAKE_CXX_STANDARD 14)

set(lib_files
  capitalism_simulator.cpp
  config_parser.cpp
)

add_library(lib SHARED ${lib_files})
target_include_directories(lib PUBLIC
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  )

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} lib)
