#include "board_functions.h"


int invalid_position(board_s *board_config, struct position pos){
  return ((pos.x >= board_config->width)||(pos.y >= board_config->height));
}


location *location_of_position(board_s *board_config, struct position pos){

  if (invalid_position(board_config,pos)){
    return (NULL);
  }
 
  return (&(board_config->board[pos.y][pos.x]));
}


int there_is_path_card(board_s *board_config, struct position pos){
 
  location *l = location_of_position(board_config,pos);

  if (l == NULL){
    return 0;
  }

  return (l->card != NO_CARD);
}



int is_neighbour(board_s *board_config, struct position pos, enum vector v){
 
  location *loc = location_of_position(board_config,pos);

  if (loc == NULL){
    return 0;
  }

  return (loc->is_neighbour[v]);
}



int has_exit_to(board_s *board_config, struct position pos, struct position target_pos){

  int i;
  location *loc = location_of_position(board_config,pos);
  struct position next_to_pos;

  if (loc == NULL){
    return (0);
  }


  for (i = 0 ; i < 4 ; i++){

    next_to_pos = next_to_position(pos,i);

    if (next_to_pos.x == target_pos.x && next_to_pos.y == target_pos.y){
      return (loc->is_neighbour[i]);
    }
  }

  return (0);
}



/*
  Determines if the position pos and the position next to pos (from the vector v) are in the same path.
*/
int is_next_to_in_the_same_path(board_s *board_config, struct position pos, enum vector v){

  location *loc = location_of_position(board_config,pos);
  struct position neighbour_pos = next_to_position(pos,v);
  location *neighbour_loc = location_of_position(board_config,neighbour_pos);

  return (loc != NULL
          && loc->is_neighbour[v] == 1
          && neighbour_loc != NULL
          && neighbour_loc->is_neighbour[opposite_vector(v)] == 1);
}


void free_board(location **board){

  free(board[0]);
  free(board);
}




unsigned int pathset_size(board_s *board_config, struct position pos){

  struct location *repr_loc = pathset_representative(board_config,pos);

  if (repr_loc == NULL){
    return (0);
  }
 
  return (repr_loc->path_size);
}



location *pathset_representative(board_s *board_config, struct position pos){

  unsigned int nb_change_stack = 0;
  unsigned int i;
  location *change_stack[MAX_PATHSET_REPRESENTATIVE_CHANGE_STACK];
  location *loc = location_of_position(board_config,pos);

  while (loc != NULL && loc->representative != loc){

    if (nb_change_stack < MAX_PATHSET_REPRESENTATIVE_CHANGE_STACK){
      change_stack[nb_change_stack++] = loc;
    }
   
    loc = loc->representative;
  }

  for (i = 0 ; i < nb_change_stack ; i++){
    change_stack[i]->representative = loc;
  }

  return (loc);
}
  
