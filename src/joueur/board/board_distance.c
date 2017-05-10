#include "board_functions.h"





unsigned int nb_cards_to_link(board_s *board_config, struct position pos_1, struct position pos_2){

  struct position current_pos;
  struct position next_to_pos;
  location *next_to_loc;
  
  unsigned int d = 0;
  
  struct position tab_1[MAX_POS_TAB_SIZE];
  struct position tab_2[MAX_POS_TAB_SIZE];


  unsigned int nb_filled_buffer = 0;
  struct position (*filled_buffer)[] = &tab_1;
  
  unsigned int nb_current_buffer = 0; 
  struct position (*current_buffer)[] = &tab_2;

  struct position (*tmp)[] = NULL;
  
  location *loc_1 = location_of_position(board_config,pos_1);
  int i;

  if (invalid_position(board_config,pos_1) || invalid_position(board_config,pos_2)){
    return (-1);
  }

  board_config->update_nb_cards_to_link++;
  
  loc_1->update_nb_cards_to_link = board_config->update_nb_cards_to_link;
  (*filled_buffer)[nb_filled_buffer++] = pos_1;


  while (nb_filled_buffer != 0){

    nb_current_buffer = nb_filled_buffer;
    nb_filled_buffer = 0;

    tmp = current_buffer;
    current_buffer = filled_buffer;
    filled_buffer = tmp;

    d++;
    
    while (nb_current_buffer != 0){

      current_pos = (*current_buffer)[--nb_current_buffer];
      
      if (current_pos.x == pos_2.x && current_pos.y == pos_2.y){
	return (d);
      }
    
      for (i = 0 ; i < 4 ; i++){
      
	next_to_pos = next_to_position(current_pos,i);
	next_to_loc = location_of_position(board_config,next_to_pos);
      
	if (!invalid_position(board_config,next_to_pos)
	    && next_to_loc->update_nb_cards_to_link != board_config->update_nb_cards_to_link
	    && next_to_loc->card == NO_CARD
	    && next_to_loc->type != LOC_HOLE
	    && nb_filled_buffer < MAX_POS_TAB_SIZE){
	
	  next_to_loc->update_nb_cards_to_link = board_config->update_nb_cards_to_link;
	  (*filled_buffer)[nb_filled_buffer++] = next_to_pos;  
	}
      }  
    }
  }

  return (-1);
}  

  
  
  
unsigned int distance_between_paths(board_s *board_config, struct position pos_1, struct position pos_2, struct position *out_1, struct position *out_2){


  location *representative_1 = pathset_representative(board_config,pos_1);
  location *representative_2 = pathset_representative(board_config,pos_2);
  
  unsigned int nb_exits_1;
  struct position exits_1[MAX_POS_TAB_SIZE];

  unsigned int nb_exits_2;
  struct position exits_2[MAX_POS_TAB_SIZE];

  int i_1;
  int i_2;

  unsigned minimum_distance = -1;
  unsigned int current_distance;

  
  if (out_1 != NULL){
    *out_1 = (struct position) {-1,-1};
  }

  if (out_2 != NULL){
    *out_2 = (struct position) {-1,-1};
  }
  
  if (representative_1 != NULL && representative_1 == representative_2){
    return (0);
  }

  nb_exits_1 = path_exits(board_config,pos_1,MAX_POS_TAB_SIZE,exits_1);
  nb_exits_2 = path_exits(board_config,pos_2,MAX_POS_TAB_SIZE,exits_2);

  for (i_1 = 0 ; i_1 < nb_exits_1 ; i_1++){
    for (i_2 = 0 ; i_2 < nb_exits_2 ; i_2++){

      current_distance = nb_cards_to_link(board_config,exits_1[i_1],exits_2[i_2]);

      if (current_distance < minimum_distance){
	
	minimum_distance = current_distance;

	if (out_1 != NULL){
	  *out_1 = exits_1[i_1];
	}

	if (out_2 != NULL){
	  *out_2 = exits_2[i_2];
	}
      }
    }
  }

  return (minimum_distance);
}




unsigned int path_exits(board_s *board_config, struct position pos, unsigned int max_nb_exits, struct position exits[]){

  unsigned int nb_exits = 0;
  position_queue q;
  struct position current_pos;
  struct position next_to_pos;
  location *next_to_loc;
  int i ;
  
  init_queue(&q);
  board_config->update_path_exits++;

  if (!there_is_path_card(board_config,pos)){
    exits[0] = pos;
    return (1);
  }

  location_of_position(board_config,pos)->update_path_exits = board_config->update_path_exits;
  add_in_queue(pos,&q);
  
  while (!queue_is_empty(&q) && nb_exits < max_nb_exits){

    current_pos = remove_from_queue(&q);

    if (!there_is_path_card(board_config,current_pos)){
      exits[nb_exits++] = current_pos;
      
    } else {
    
      for (i = 0 ; i < 4 ; i++){

	next_to_pos = next_to_position(current_pos,i);
	next_to_loc = location_of_position(board_config,next_to_pos);
	
	if (is_neighbour(board_config,current_pos,i)
	    && next_to_loc != NULL
	    && next_to_loc->update_path_exits != board_config->update_path_exits){
	  
	  next_to_loc->update_path_exits = board_config->update_path_exits;
	  add_in_queue(next_to_pos,&q);
	  
	}
      }
    }
  }

  return (nb_exits);
}




