add_executable(GameClient
    src/main.cpp
    src/glad.c
    src/utils/Mesh.cpp
    src/utils/Shader.cpp
    src/utils/io.cpp
    # Add other source files here
)

file(COPY ${CMAKE_SOURCE_DIR}/client/assets DESTINATION ${CMAKE_BINARY_DIR}/client)

target_include_directories(GameClient PRIVATE
    "${CMAKE_SOURCE_DIR}/client/dependencies/include"
)

target_link_libraries(GameClient
    "${CMAKE_SOURCE_DIR}/client/dependencies/lib/libglfw3.a"
)