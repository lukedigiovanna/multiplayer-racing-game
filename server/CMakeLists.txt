add_executable(GameServer
    src/main.cpp
    src/Connection.cpp
    src/Player.cpp
    src/mathutils.hpp
)

target_include_directories(GameServer PRIVATE
    "${CMAKE_SOURCE_DIR}/server/dependencies/include"
)

target_link_libraries(GameServer
    # Add any other libraries needed by the server
)
