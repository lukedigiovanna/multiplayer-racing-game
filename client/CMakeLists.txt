add_executable(GameClient
    src/main.cpp
    src/glad.c
    # Add other source files here
)

target_include_directories(GameClient PRIVATE
    "${CMAKE_SOURCE_DIR}/client/dependencies/include"
)

target_link_libraries(GameClient
    "${CMAKE_SOURCE_DIR}/client/dependencies/lib/libglfw3.a"
)