cmake_minimum_required(VERSION 2.8.11)

project(SAPOTACHE)

#add_library(client1 ./joueur/player.c)
#
#add_executable(serveur ./serveur/serveur.c
#  ./serveur/serveur.h
#  ./serveur/initialize.c
#  ./serveur/play_move.c
#  ./error/error_and_free.c
#  ./error/error.h
#  ./error/print_error.c
#  ./parseur/lexer.c
#  ./parseur/parse_functions.c
#  ./parseur/parseur.h
#  ./parseur/parseur.c
#  ./parseur/print_parse.c
#  ./joueur/board/board_basic.c
#  ./joueur/board/board_distance.c
#  ./joueur/board/board_free.c
#  ./joueur/board/board_functions.h
#  ./joueur/board/board_initialization.c
#  ./joueur/board/board_play.c
#  ./joueur/board/board_structure.h
#  ./joueur/board/board_update.c
#  ./joueur/board/position.c
#  ./joueur/board/position.h
#  ./serveur/board/board_interface.h
#  )

file(

        GLOB_RECURSE

        source_files

	../src/error/*
	../src/serveur/*
	../src/parseur/*
	../src/joueur/board/*	
)

add_executable(

  my_exe

  ${source_files} 

  #./serveur/board/board_interface.h
)

set(EXECUTABLE_OUTPUT_PATH ../bin)

