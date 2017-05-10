#include "../../interface/interface.h"
#include "position.h"



void init_queue(position_queue *q){
  q->input = 0;
  q->output = 0;
}

int queue_is_empty(position_queue *q){
  return (q->input == q->output);
}

int queue_is_full(position_queue *q){
  return ((q->output - q->input + MAX_QUEUE_SIZE)%MAX_QUEUE_SIZE == 1); 
}


void add_in_queue(struct position pos, position_queue *q){
  //ERREUR 
  if (queue_is_full(q)){
    return;
  }

  q->content[q->input] = pos;
  q->input = (q->input + 1)%MAX_QUEUE_SIZE;
} 


struct position remove_from_queue(position_queue *q){ 

  struct position pos;
  
  //ERREUR
  if (queue_is_empty(q)){ 
    return ((struct position) {-1,-1}); 
  }

  pos = q->content[q->output]; 
  q->output = (q->output + 1)%MAX_QUEUE_SIZE; 

  return (pos); 
} 



struct position next_to_position(struct position pos, enum vector v){

  struct position out = {pos.x, pos.y};

  switch (v){

  case UP:
    out.y++;
    break;

  case DOWN:
    out.y--;
    break;

  case LEFT:
    out.x--;
    break;

  case RIGHT:
    out.x++;
    break;
  }

  return (out);
}


enum vector opposite_vector(enum vector v){
  return (v + (v%2 == 0) - (v%2 == 1));
}


unsigned int neighbours_for_card(enum card_id card, enum direction direction, int output_tab[4]){


  unsigned int nb_neighbours = 0;
  int tmp;
  static int no_card[4] = {0,0,0,0};
  static int v_line[4] = {1,1,0,0};
  static int h_line[4] = {0,0,1,1};
  static int v_cross[4] = {1,1,1,0};
  static int h_cross[4] = {1,0,1,1};
  static int x_cross[4] = {1,1,1,1};
  static int l_turn[4] = {0,1,1,0};
  static int r_turn[4] = {0,1,0,1};
  static int d_end[4] = {0,0,0,0};
  

  switch (card){

  case NO_CARD:
    memcpy(output_tab,no_card,4*sizeof(int));
    nb_neighbours = 0;
    break;

  case CARD_V_LINE:
    memcpy(output_tab,v_line,4*sizeof(int));
    nb_neighbours = 2;
    break;

  case CARD_H_LINE:
    memcpy(output_tab,h_line,4*sizeof(int));
    nb_neighbours = 2;
    break;

  case CARD_V_CROSS:
    memcpy(output_tab,v_cross,4*sizeof(int));
    nb_neighbours = 3;
    break;

  case CARD_H_CROSS:
    memcpy(output_tab,h_cross,4*sizeof(int));
    nb_neighbours = 3;
    break;

  case CARD_X_CROSS:
    memcpy(output_tab,x_cross,4*sizeof(int));
    nb_neighbours = 4;
    break;

  case CARD_L_TURN:
    memcpy(output_tab,l_turn,4*sizeof(int));
    nb_neighbours = 2;
    break;

  case CARD_R_TURN:
    memcpy(output_tab,r_turn,4*sizeof(int));
    nb_neighbours = 2;
    break;

  case CARD_D_END:
    memcpy(output_tab,d_end,4*sizeof(int));
    nb_neighbours = 0;
    break;
    
  default:
    fprintf(stderr,"Card_to_exists: unvalid card %d.\n",(int) card);
    exit(EXIT_FAILURE);
  }

  if (direction == REVERSED){

    tmp = output_tab[0];
    output_tab[0] = output_tab[1];
    output_tab[1] = tmp;
    
    tmp = output_tab[2];
    output_tab[2] = output_tab[3];
    output_tab[3] = tmp;
  }

  return (nb_neighbours);
}
