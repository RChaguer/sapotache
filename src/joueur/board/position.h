#ifndef SAPOTACHE_POSITION_H
#define SAPOTACHE_POSITION_H

#include "../../interface/interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum vector {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT
};


#define MAX_POS_TAB_SIZE 5000
#define MAX_QUEUE_SIZE 5000

struct position_queue {
  
  unsigned int input;
  unsigned int output; 
  
  struct position content[MAX_QUEUE_SIZE];
  
};

typedef struct position_queue position_queue;


void init_queue(position_queue *q);

int queue_is_empty(position_queue *q);

int queue_is_full(position_queue *q);

void add_in_queue(struct position pos, position_queue *q);

struct position remove_from_queue(position_queue *q);



struct position next_to_position(struct position pos, enum vector v);

enum vector opposite_vector(enum vector v);

unsigned int neighbours_for_card(enum card_id card, enum direction direction, int output_tab[4]);





#endif //SAPOTACHE_POSITION_H
