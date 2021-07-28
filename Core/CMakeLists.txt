cmake_minimum_required(VERSION 3.14)
project(oled-bmp-ut)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLAGS -Wall -Werror -Wextra -pedantic -g)

include_directories(Inc)

add_subdirectory(Src)
add_subdirectory(UT)
