add_executable(GameClient
    src/main.cpp
    # Add other source files here
)

target_include_directories(GameClient PRIVATE
    include
)

target_link_libraries(GameClient
    # Add any other libraries needed by the client
)