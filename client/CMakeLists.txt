add_executable(GameClient
    src/main.cpp
    src/glad.c
    src/utils/Mesh.cpp
    src/utils/Shader.cpp
    src/utils/io.cpp
    src/utils/input.cpp
    # Add other source files here
)

file(COPY ${CMAKE_SOURCE_DIR}/client/assets DESTINATION ${CMAKE_BINARY_DIR}/client)

target_include_directories(GameClient PRIVATE
    "${CMAKE_SOURCE_DIR}/client/dependencies/include"
)

target_link_libraries(GameClient
    "${CMAKE_SOURCE_DIR}/client/dependencies/lib/libglfw3.a"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(APPKIT_FRAMEWORK AppKit)
    find_library(CORE_GRAPHICS_FRAMEWORK CoreGraphics)

    target_link_libraries(GameClient
        "${CORE_FOUNDATION_FRAMEWORK}"
        "${IOKIT_FRAMEWORK}"
        "${APPKIT_FRAMEWORK}"
        "${CORE_GRAPHICS_FRAMEWORK}"
    )
endif()