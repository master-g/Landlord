cmake_minimum_required(VERSION 3.4)
project(Landlord)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=gnu99 -O0")

set(SOURCE_FILES
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
        src/main.c
        src/lmath.c
        src/lmath.h
        src/ruiko_algorithm.c
        src/ruiko_algorithm.h
        src/memtracker.c
        src/memtracker.h
        src/player.c
        src/player.h
        src/standard_ai.c
        src/standard_ai.h
        src/handlist.h
        src/handlist.c
        src/landlord.h)

add_executable(Landlord ${SOURCE_FILES})