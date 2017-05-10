#include "board_functions.h"



void update_board(board_s *board_config, struct move const previous_moves[], size_t n_moves){
  
  unsigned int i;
  struct move const *m = NULL;
  
  for (i = 0 ; i < n_moves ; i++){

    m = previous_moves + i;

    switch (m->act){

    case ADD_PATH_CARD:
      put_card(board_config,m->onto,m->card,m->dir);
      break;

    case PLAY_BOULDER_CARD:
      put_card(board_config,m->onto,NO_CARD,NORMAL);
      break;

    default:
      break;
    }
  }
}



void put_card(board_s *board_config, struct position pos, enum card_id card, enum direction direction){

  location *loc = location_of_position(board_config,pos);

  if (loc == NULL){
    return;
  }
 
  loc->card = card;
  loc->direction = direction; 
  loc->nb_neighbours = neighbours_for_card(card,direction,loc->is_neighbour);
  loc->visited = 1;
  
  if (card == NO_CARD){
    update_pathset_boulder(board_config,pos);
  } else {
    update_pathset_new_card(board_config,pos);
  }
}



void update_pathset_boulder(board_s *board_config, struct position pos){

  int i ;
  struct position neighbour_pos;
  location *loc = location_of_position(board_config,pos);
  location *neighbour_loc = NULL;

  if (loc != NULL){
    loc->representative = NULL;
  }

  for (i = 0 ; i < 4 ; i++){

    neighbour_pos = next_to_position(pos,i);

    if (is_neighbour(board_config,neighbour_pos,opposite_vector(i))){
      spread_representative(board_config,neighbour_pos,NULL);      
    }
  }

  for (i = 0 ; i < 4 ; i++){

    neighbour_pos = next_to_position(pos,i);
    neighbour_loc = location_of_position(board_config,neighbour_pos);

    if (is_neighbour(board_config,neighbour_pos,opposite_vector(i))
	&& neighbour_loc->representative == NULL){
      neighbour_loc->path_size = 0;
      neighbour_loc = location_of_position(board_config,neighbour_pos);
      spread_representative(board_config,neighbour_pos,neighbour_loc);
    }  
  }
}


void spread_representative(board_s *board_config, struct position pos, location *new_representative){

  int i;
  struct position neighbour_pos;
  location *loc = location_of_position(board_config,pos);

  if (loc == NULL || loc->representative == new_representative){
    return;
  }

  loc->representative = new_representative;

  if (new_representative != NULL){
    new_representative->path_size++;
  }
  
  for (i = 0 ; i < 4 ; i++){

    if (is_next_to_in_the_same_path(board_config,pos,i)){
      neighbour_pos = next_to_position(pos,i);
      spread_representative(board_config,neighbour_pos,new_representative);
    }
  }
}



void update_pathset_new_card(board_s *board_config, struct position pos){

  int i;
  location *neighbour_representative_loc = NULL;
  struct position neighbour_pos;
  enum vector v;
  unsigned int max_path_size = 0;
  unsigned int path_size;
  
  for (i = 0 ; i < 4 ; i++){

    if (is_next_to_in_the_same_path(board_config,pos,i)){

      path_size = pathset_size(board_config,next_to_position(pos,i));

      if (path_size > max_path_size){
	max_path_size = path_size;
	v = i;
      }
    }
  }
  
  if (max_path_size != 0){
    
    neighbour_pos = next_to_position(pos,v);
    neighbour_representative_loc = pathset_representative(board_config,neighbour_pos);
    change_pathset_representative(board_config,pos,neighbour_representative_loc);
    
  } else {

    neighbour_representative_loc = location_of_position(board_config,pos);
    neighbour_representative_loc->representative = neighbour_representative_loc;
    neighbour_representative_loc->path_size = 1;
  }
}

void change_pathset_representative(board_s *board_config, struct position pos, location *new_representative){

  location *loc = location_of_position(board_config,pos);
  location *neighbour_representative_loc = NULL;
  struct position neighbour_pos;
  int i;
  
  if (loc == NULL){
    return;
  }

  loc->representative = new_representative;

  if (new_representative != NULL){
    new_representative->path_size++;
  }

  for (i = 0 ; i < 4 ; i++){

    neighbour_pos = next_to_position(pos,i);
    
    if (is_next_to_in_the_same_path(board_config,pos,i)){
      
      neighbour_representative_loc = pathset_representative(board_config,neighbour_pos);

      if (neighbour_representative_loc != new_representative
	  && neighbour_representative_loc != NULL){
	new_representative->path_size += neighbour_representative_loc->path_size;
	neighbour_representative_loc->representative = new_representative;
      }    
    }
  }
}
