#include "board_functions.h"


void print_normal_card(enum card_id card){

  switch (card){

  case CARD_V_CROSS:
    printf("-| ");
    return;

  case CARD_H_CROSS:
    printf("_|_");
    return;

  case CARD_L_TURN:
    printf(" 7 ");
    return;

  case CARD_R_TURN:
    printf(" F ");
    return;

  default:
    printf("Err");
    return;

  }

}


void print_reversed_card(enum card_id card){

  switch (card){

  case CARD_V_CROSS:
    printf(" |-");
    return;

  case CARD_H_CROSS:
    printf(" T ");
    return;

  case CARD_L_TURN:
    printf(" L ");
    return;

  case CARD_R_TURN:
    printf("_| ");
    return;

  default:
    printf("Err");
    return;

  }

}



void print_card(enum card_id card, enum direction direction){

  switch (card){

  case NO_CARD:
    printf(" . ");
    return;

  case CARD_V_LINE:
    printf(" | ");
    return;

  case CARD_H_LINE:
    printf("---");
    return;
    
  case CARD_X_CROSS:
    printf("-|-");
    return;

  case CARD_D_END:
    printf("[ ]");
    return;

  default:

    switch (direction){

    case NORMAL:
      print_normal_card(card);
      return;

    case REVERSED:
      print_reversed_card(card);
      return;
   
    default:
      printf("Err");
      return;
    }
  }
}


void print_board(board_s *board_config){

  int i;
  int j;
  location *loc = NULL;
  struct position pos ;
  
  printf("\n");

  for (i = board_config->height-1 ; i >= 0  ; i--){

    pos.y = i;

    for (j = 0 ; j < board_config->width ; j++){
      pos.x = j;      
      loc = location_of_position(board_config,pos);
      print_card(loc->card,loc->direction);

      switch(loc->type){

      case LOC_START:
	printf(">");
	break;

      case LOC_HOLE:
	printf("%%");
	break;

      case LOC_OBJECTIVE:
	printf("$");
	break;
	
      default:
	printf(" ");
	break;
      }
      
      printf("(%u,%d)  ",pathset_size(board_config,pos),is_linked_to_start(board_config,pos));
    }
    
    printf("\n");
  }
  
}
