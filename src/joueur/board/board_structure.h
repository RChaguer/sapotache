#ifndef SAPOTACHE_PLAYER_BOARD_STRUCTURE
#define SAPOTACHE_PLAYER_BOARD_STRUCTURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../interface/interface.h"
#include "position.h"


#define MAX_PATHSET_REPRESENTATIVE_CHANGE_STACK 32
#define MAX_NB_GUESS_PILE 500

enum location_type {

  LOC_NOTHING = 0,
  LOC_START,
  LOC_HOLE,
  LOC_OBJECTIVE
  
};


struct location {

  enum location_type type;
  
  enum card_id card;
  enum direction direction;

  unsigned int nb_neighbours;
  
  int is_neighbour[4];
  
  struct location *representative;
  unsigned int path_size;

  unsigned long long int update_nb_cards_to_link;
  unsigned long long int update_path_exits;
  unsigned long long int update_possible_moves_for_card;
  
  int visited;
};

typedef struct location location ;


struct guess {

  struct position position;
  enum card_id card;
  enum direction direction;
  
};

typedef struct guess guess;

  
struct board_config {

  unsigned int width;
  unsigned int height;
  location **board;
  
  struct position start;
  size_t n_objectives;
  struct position *objectives;
  size_t n_holes;
  struct position *holes;

  unsigned int nb_guess_pile;
  guess guess_pile[MAX_NB_GUESS_PILE];
  
  unsigned long long int update_nb_cards_to_link;
  unsigned long long int update_path_exits;
  unsigned long long int update_possible_moves_for_card;
  
};

typedef struct board_config board_s;

#endif //SAPOTACHE_PLAYER_BOARD_STRUCTURE
