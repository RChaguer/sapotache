#include "player_data_functions.h"


#define CASE_MATCH(name) case CARD_##name: return (#name);


char *str_of_card_id(enum card_id card){

  switch (card){

    CASE_MATCH(V_LINE)
    CASE_MATCH(H_LINE)
    CASE_MATCH(V_CROSS)
    CASE_MATCH(H_CROSS)
    CASE_MATCH(X_CROSS)
    CASE_MATCH(L_TURN)
    CASE_MATCH(R_TURN)
    CASE_MATCH(D_END)
    CASE_MATCH(BOULDER)
    CASE_MATCH(B_AXE)
    CASE_MATCH(B_LAMP)
    CASE_MATCH(B_CART)
    CASE_MATCH(R_AXE)
    CASE_MATCH(R_LAMP)
    CASE_MATCH(R_CART)
    CASE_MATCH(R_ALL)
      
  default:
    return "NO_CARD";

  }
}


void print_status(play_data *data){

  unsigned int i;
  
  printf("Player cards : \n");

  for (i = 0 ; i < data->n_player_cards ; i++){
    printf("%s ",str_of_card_id(data->player_cards[i]));
  }
  
  printf("\n");
}
