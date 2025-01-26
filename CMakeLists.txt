cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES}
        src/builtins/builtins.cpp
        src/builtins/builtins.hpp
        src/builtins/cd/cd.cpp
        src/builtins/cd/cd.hpp
        src/builtins/echo/echo.cpp
        src/builtins/echo/echo.hpp
        src/builtins/exit/exit.hpp
        src/builtins/exit/exit.cpp
        src/builtins/pwd/pwd.cpp
        src/builtins/pwd/pwd.hpp
        src/builtins/type/type.cpp
        src/builtins/type/type.hpp
)
