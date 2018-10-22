set (CMAKE_CXX_STANDARD 11)

project(Capitalism)

set(FILES
  src/main.cpp
  src/capitalismClass.cpp
  src/capitalismClass.h
)

add_executable(main ${FILES})
