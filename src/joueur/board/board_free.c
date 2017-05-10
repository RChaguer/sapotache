#include "board_functions.h"



void free_board_config(board_s *board_config){

  free(board_config->objectives);
  free(board_config->holes);
  free((board_config->board)[0]);
  free(board_config->board);
  
}


void free_server_board_config(board_s *board_config){

  free(board_config->objectives);
  free(board_config->holes);
  free((board_config->board)[0]);
  free(board_config->board);
  free(board_config);
  
}
