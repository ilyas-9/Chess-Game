cmake_minimum_required(VERSION 3.31)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

# Find SFML package

add_executable(Chess
        src/main.cpp
        src/Chess.cpp
        src/Chess.h)

# add SFML subdirectory
add_subdirectory(external/SFML-2.6.1)

target_link_libraries(Chess PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Copy assets into the same dir as the binary
add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/chess-items
        ${CMAKE_CURRENT_BINARY_DIR}/chess-items)
