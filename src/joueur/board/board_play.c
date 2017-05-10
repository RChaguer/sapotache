#include "board_functions.h"



int can_put_card_here(board_s *board_config, struct position pos, enum card_id card, enum direction dir){

  int next_to_a_path = 0;
  unsigned int i;
  struct position next_to_pos;
  int has_exits[4];
  location *loc = NULL;
  int leads_to_pos;
  
  if (invalid_position(board_config,pos)
      || (loc = location_of_position(board_config,pos))->card != NO_CARD
      || loc->type == LOC_HOLE){

    return (0);
  }

  neighbours_for_card(card,dir,has_exits);

  for (i = 0 ; i < 4 ; i++){

    next_to_pos = next_to_position(pos,i);

    if (there_is_path_card(board_config,next_to_pos)){

      leads_to_pos = is_neighbour(board_config,next_to_pos,opposite_vector(i));
      
      if (leads_to_pos){
	next_to_a_path = 1;
      }

      if (has_exits[i] != leads_to_pos){
	return (0);
      }
    }
  }
  
  return (next_to_a_path);
}




int is_linked_to_start(board_s *board_config, struct position pos){

  location *start_representative = pathset_representative(board_config,board_config->start);
  location *pos_representative = pathset_representative(board_config,pos);

  return (pos_representative != NULL && start_representative == pos_representative);
}


unsigned int possible_moves_for_card(board_s *board_config, struct position pos[], enum direction dir[], unsigned int nb, enum card_id card){

  unsigned int nb_possible_moves = 0;
  struct position current_pos;
  struct position next_to_pos;
  location *next_to_loc = NULL;
  position_queue q;
  int i;
  
  init_queue(&q);
  board_config->update_possible_moves_for_card++;
  
  location_of_position(board_config,board_config->start)->update_possible_moves_for_card = board_config->update_possible_moves_for_card;
  add_in_queue(board_config->start,&q);  

  while (nb_possible_moves < nb && !queue_is_empty(&q)){

    current_pos = remove_from_queue(&q);

    if (can_put_card_here(board_config,current_pos,card,NORMAL)){
      pos[nb_possible_moves] = current_pos;
      dir[nb_possible_moves] = NORMAL;
      nb_possible_moves++;
    }

    if (can_put_card_here(board_config,current_pos,card,REVERSED)
	&& nb_possible_moves < nb){
      pos[nb_possible_moves] = current_pos;
      dir[nb_possible_moves] = REVERSED;
      nb_possible_moves++;
    }

    for (i = 0 ; i < 4 ; i++){

      next_to_pos = next_to_position(current_pos,i);
      next_to_loc = location_of_position(board_config,next_to_pos);

      if (next_to_loc == NULL || next_to_loc->update_possible_moves_for_card == board_config->update_possible_moves_for_card){
	continue;
      }
      
      if ((next_to_loc->visited || is_neighbour(board_config,current_pos,i))
	  &&(!queue_is_full(&q))){
	next_to_loc->update_possible_moves_for_card = board_config->update_possible_moves_for_card;
	add_in_queue(next_to_pos,&q);
      }
    }
  }
  
  return (nb_possible_moves);
}
