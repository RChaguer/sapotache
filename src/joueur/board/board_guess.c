#include "board_functions.h"



void add_guess(board_s *board_config, enum card_id card, struct position pos, enum direction dir){

  guess g;
  location *loc = location_of_position(board_config,pos);
  
  //ERREUR A GERER
  if (board_config->nb_guess_pile >= MAX_NB_GUESS_PILE){
    fprintf(stderr,"add_guess : pile is full\n");
    return;
  }

  if (loc == NULL){
    return;
  }

  g.position = pos;
  g.card = loc->card;
  g.direction = loc->direction;

  board_config->guess_pile[board_config->nb_guess_pile++] = g;
  
  if (card == CARD_BOULDER){
    put_card(board_config,pos,NO_CARD,NORMAL);
  } else {
    put_card(board_config,pos,card,dir);
  }
}


void undo_last_guess(board_s *board_config){

  guess g;

  if (board_config->nb_guess_pile == 0){
    return;
  }

  g = board_config->guess_pile[--board_config->nb_guess_pile];
  put_card(board_config,g.position,g.card,g.direction);
  
}



void undo_all_guess(board_s *board_config){

  while (board_config->nb_guess_pile != 0){
    undo_last_guess(board_config);
  }

}
