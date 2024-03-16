add_executable(GameServer
    src/main.cpp
    # Add other source files here
)

target_include_directories(GameServer PRIVATE
    include
)

target_link_libraries(GameServer
    # Add any other libraries needed by the server
)
