cmake_minimum_required(VERSION 3.16)

project(Racer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# DEBUG
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(client)
add_subdirectory(server)
