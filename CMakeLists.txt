cmake_minimum_required(VERSION 4.3)
project(c_proj C)

set(CMAKE_C_STANDARD 11)

add_executable(c_proj
        src/main.c
        src/map.c
        src/player.c
        src/inventory.c
        src/shop.c
)

# ncurses appeared to not be in windows :(
if(WIN32)
    target_link_libraries(c_proj pdcurses)
else()
    target_link_libraries(c_proj ncurses)
endif()
