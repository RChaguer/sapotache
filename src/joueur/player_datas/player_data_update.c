#include "player_data_functions.h"



void remove_card(play_data *data, enum card_id card){

  unsigned int i = 0;

  while (i < data->n_player_cards && data->player_cards[i] != card){
    i++;
  }

  //ERREUR A GERER AUTREMENT
  if (i == data->n_player_cards){
    fprintf(stderr,"remove_card : card not owned by the player\n");
    return;
  }

  data->n_player_cards--;
  
  while (i < data->n_player_cards){
    data->player_cards[i] = data->player_cards[i+1];
    i++;
  }
}


void add_card(play_data *data, enum card_id card){
  data->player_cards[data->n_player_cards++] = card;
}


void update_play_data(play_data *data, struct move const previous_moves[], size_t n_moves){

  unsigned int i;
  struct move m;
  
  for (i = 0 ; i < n_moves ; i++){

    m = previous_moves[i];
    
    switch(m.act){

    case PLAY_BREAK_CARD:
      update_break_tool(data,m.card,m.onplayer);
      break;
      
    case PLAY_REPAIR_CARD:
      update_repair_tool(data,m.card,m.onplayer);
      break;
      
    default:
      break;

    }
  }
}


void update_break_tool(play_data *data, enum card_id card, unsigned int targeted_player){

  switch(card){

  case CARD_B_AXE:
    data->players[targeted_player].broken_axe = 1;
    return;

  case CARD_B_LAMP:
    data->players[targeted_player].broken_lamp = 1;
    return;

  case CARD_B_CART:
    data->players[targeted_player].broken_cart = 1;
    return;

    //ERREUR A GERER
  default:
    fprintf(stderr,"break tool : wrong card\n");
    return;
  } 
}



void update_repair_tool(play_data *data, enum card_id card, unsigned int targeted_player){

  switch(card){
    
  case CARD_R_AXE:
    data->players[targeted_player].broken_axe = 0;
    return;

  case CARD_R_LAMP:
    data->players[targeted_player].broken_lamp = 0;
    return;

  case CARD_R_CART:
    data->players[targeted_player].broken_cart = 0;
    return;

  case CARD_R_ALL:
    data->players[targeted_player].broken_axe = 0;
    data->players[targeted_player].broken_lamp = 0;
    data->players[targeted_player].broken_cart = 0;
    return;
    
    //ERREUR A GERER
  default:
    fprintf(stderr,"repair tool : wrong card\n");
    return;
  } 
}
