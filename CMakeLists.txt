cmake_minimum_required(VERSION 3.15)
project(Yukon_Solitair)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories("src")
include_directories(gui_py)

add_executable(Yukon_Solitair
        "src/card.c"

        "src/game.c"

        "src/io.c"

        "src/main.c"
        "src/utils.c"

        gui_py/Client.py
        gui_py/gui.py)