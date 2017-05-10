#include "board_functions.h"


board_s *new_board_config(unsigned int width,
			  unsigned int height,
			  struct position start,
			  size_t n_objectives,
			  struct position const objectives[],
			  size_t n_holes,
			  struct position const holes[]){


  board_s *board_config = malloc(sizeof(struct board_config));

  // ERREUR A GERER AUTREMENT
  if (board_config == NULL){
    fprintf(stderr,"new_board_config : malloc error\n");
    return NULL;
  }

  initialize_board(board_config,width,height,start,n_objectives,objectives,n_holes,holes);

  return (board_config);
}


void initialize_board(board_s *board_config,
		      unsigned int width,
		      unsigned int height,
		      struct position start,
		      size_t n_objectives,
		      struct position const objectives[],
		      size_t n_holes,
		      struct position const holes[]){

  board_config->width = width;
  board_config->height = height;
  board_config->board = make_board(width,height);

  initialize_start(board_config,start);
  initialize_objectives(board_config,n_objectives,objectives);
  initialize_holes(board_config,n_holes,holes);

  board_config->nb_guess_pile = 0;
  
  board_config->update_nb_cards_to_link = 0;
  board_config->update_path_exits = 0;
  board_config->update_possible_moves_for_card = 0;
  
}


void set_location_type(board_s *board_config, struct position pos, enum location_type type){
  board_config->board[pos.y][pos.x].type = type;
}





location **make_board(unsigned int width, unsigned int height){

  unsigned int i;
  unsigned int j;
  location *t = malloc(width*height*sizeof(struct location));
  location **board = malloc(height*sizeof(struct location*));

  // ERREUR A GERER AUTREMENT
  if (t == NULL || board == NULL){
    fprintf(stderr,"Initialize: Couldn't allocate the board.\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0 ; i < height ; i++){
    board[i] = t + width*i;
  }

  for (i = 0 ; i < height ; i++){

    for (j = 0 ; j < width ; j++){
      new_empty_location(&(board[i][j]));
    }
  }

  return (board);
}


void new_empty_location(location *loc){

  int i;

  if (loc == NULL){
    return;
  }

  loc->type = LOC_NOTHING;

  loc->card = NO_CARD;
  loc->direction = NORMAL;
  loc->nb_neighbours = 0;

  for (i = 0 ; i < 4 ; i++){
    loc->is_neighbour[i] = 0;
  }

  loc->representative = NULL;
  loc->path_size = 0;

  loc->visited = 0;
  loc->update_nb_cards_to_link = 0;
  loc->update_path_exits = 0;
  loc->update_possible_moves_for_card = 0;
}


// TODO : update counter ?
void initialize_start(board_s *board_config, struct position start){


  location *loc = location_of_position(board_config,start);

  board_config->start = start;
  
  set_location_type(board_config,start,LOC_START);
  put_card(board_config,start,CARD_X_CROSS,NORMAL);

  loc->representative = loc;
  loc->path_size = 1;

  loc->update_nb_cards_to_link = 0;
  loc->update_path_exits = 0;
  loc->update_possible_moves_for_card = 0;
}


void initialize_objectives(board_s *board_config, size_t n_objectives, struct position const objectives[]){
			     
  unsigned int i;
  struct position pos;
  
			   
  board_config->n_objectives = n_objectives;
  board_config->objectives = malloc(sizeof(struct position) * n_objectives);

  //GERER ERREUR DIFFEREMMENT
  if (board_config->objectives == NULL){
    fprintf(stderr,"Initialize : Couldn't allocate the objectives board.\n");
    exit(EXIT_FAILURE);
  }
  
  for (i = 0 ; i < n_objectives ; i++){

    pos = objectives[i];
    board_config->objectives[i] = pos;
    set_location_type(board_config,pos,LOC_OBJECTIVE);
  }
}


void initialize_holes(board_s *board_config, size_t n_holes, struct position const holes[]){

  unsigned int i;
  struct position pos;
 
  board_config->n_holes = n_holes;
  board_config->holes = malloc(sizeof(struct position) * n_holes);

  // ERREUR A GERER DIFFEREMMENT
  if (board_config->holes == NULL){
    fprintf(stderr,"Initialize : Couldn't allocate the holes board.\n");
    exit(EXIT_FAILURE);
  }
  
  for (i = 0 ; i < n_holes ; i++){
    
    pos = holes[i];
    board_config->holes[i] = pos;
    set_location_type(board_config,pos,LOC_HOLE);
  }
}
