cmake_minimum_required(VERSION 3.15)
project(Yukon_Solitair C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Yukon_Solitair

        src/server.c
        src/game.c
        src/card.c
        src/io.c
        src/utils.c
)
