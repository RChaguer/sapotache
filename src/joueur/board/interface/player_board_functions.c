#include "../board_functions.h"


static board_s board_config;


void board__initialize(unsigned int width,
		       unsigned int height,
		       struct position start,
		       size_t n_objectives,
		       struct position const objectives[],
		       size_t n_holes,
		       struct position const holes[]){

  initialize_board(&board_config,width,height,start,n_objectives,objectives,n_holes,holes);
}


void board__update(struct move const previous_moves[], size_t n_moves){
  update_board(&board_config,previous_moves,n_moves);
}


void board__free(){
  free_board_config(&board_config);
}


int board__invalid_pos(struct position pos){
  return invalid_position(&board_config,pos);
}


// Determines if the position next to pos is reachable from pos  
int board__is_next_pos_neighbour(struct position pos, enum vector v){
  return is_neighbour(&board_config,pos,v);
};


int board__can_put_card_here(struct position pos, enum card_id card, enum direction dir){
  return can_put_card_here(&board_config,pos,card,dir);
}

 
enum card_id board__card_on_pos(struct position pos){

  location *loc = location_of_position(&board_config,pos);
  
  if (loc == NULL){
    return (NO_CARD);
  }

  return (loc->card);
}


enum direction board__dir_on_pos(struct position pos){
  
  location *loc = location_of_position(&board_config,pos);
  
  if (loc == NULL){
    return (NORMAL);
  }

  return (loc->direction);
}


unsigned int board__neighbours_list(struct position pos, struct position neighbours_tab[]){

  int i;
  unsigned int nb_neighbours = 0;

  for (i = 0 ; i < 4 ; i++){

    if (is_neighbour(&board_config,pos,i)){
      neighbours_tab[nb_neighbours++] = next_to_position(pos,i);
    }
  }
  
  return (nb_neighbours);
}


int board__is_linked_to_start(struct position pos){
  return is_linked_to_start(&board_config,pos);
}


int board__is_linked_to_path(struct position pos){

  int i;
  struct position next_to_pos;
  
  if (there_is_path_card(&board_config,pos)){
    return (1);
  }

  for (i = 0 ; i < 4 ; i++){
    
    next_to_pos = next_to_position(pos,i);
    
    if (is_neighbour(&board_config,next_to_pos,opposite_vector(i))){
      return (1);
    }
  }

  return (0);
}

unsigned int board__distance_to_start(struct position pos){
  return (distance_between_paths(&board_config,board_config.start,pos,NULL,NULL));
}


struct position board__start(){
  return (board_config.start);
}


struct position *board__obj_list(){
  return (board_config.objectives);
}


unsigned int board__nb_obj(){
  return (board_config.n_objectives);
}


unsigned int board__distance_to_objective(struct position pos, unsigned int o){
  return (distance_between_paths(&board_config,pos,board_config.objectives[o],NULL,NULL));
}


unsigned int board__possible_moves_for_card(struct position pos[], enum direction dir[], unsigned int nb, enum card_id card){
  return (possible_moves_for_card(&board_config,pos,dir,nb,card));
}


void board__guess(enum card_id card, struct position pos, enum direction dir){
  add_guess(&board_config,card,pos,dir);
}

void board__guess_undo_all(){
  undo_all_guess(&board_config);
}

void board__guess_undo_once(){
  undo_last_guess(&board_config);
}

enum card_id blocked_objective(unsigned int o);


void board__print(){
  print_board(&board_config);
}
