set (CMAKE_CXX_STANDARD 11)

project(capitalism_simulator)

set(FILES
  main.cpp
  capitalismClass.cpp
  capitalismClass.h
)

add_executable(${PROJECT_NAME} ${FILES})
