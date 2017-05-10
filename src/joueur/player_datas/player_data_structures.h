#ifndef SAPOTACHE_PLAYER_DATA_STRUCTURES_H
#define SAPOTACHE_PLAYER_DATA_STRUCTURES_H

#include "../../interface/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 128

struct player_information {

  int broken_axe;
  int broken_cart;
  int broken_lamp;
  
};

typedef struct player_information player_information;


struct play_data {

  unsigned int id;
  enum p_type type;
  char name[MAX_STR_LENGTH];
  
  unsigned int n_cards_total;
  
  size_t n_player_cards;
  enum card_id *player_cards;

  unsigned int n_players;
  player_information *players;
  
};

typedef struct play_data play_data;




#endif //SAPOTACHE_PLAYER_DATA_STRUCTURES_H
