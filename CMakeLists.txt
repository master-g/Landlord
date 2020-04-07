cmake_minimum_required(VERSION 3.15)
project(Landlord C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Landlord
        src/advanced_ai.c
        src/advanced_ai.h
        src/card.c
        src/card.h
        src/common.h
        src/deck.c
        src/deck.h
        src/game.c
        src/game.h
        src/hand.c
        src/hand.h
        src/handlist.c
        src/handlist.h
        src/landlord.h
        src/lmath.c
        src/lmath.h
        src/main.c
        src/memtracker.c
        src/memtracker.h
        src/player.c
        src/player.h
        src/ruiko_algorithm.c
        src/ruiko_algorithm.h
        src/standard_ai.c
        src/standard_ai.h)
