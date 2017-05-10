#ifndef SAPOTACHE_BOARD_FUNCTIONS_H
#define SAPOTACHE_BOARD_FUNCTIONS_H

#include "board_structure.h"


/*
 * File board_basic.c
 */

int invalid_position(board_s *board_config, struct position pos);

location *location_of_position(board_s *board_config, struct position pos);

int there_is_path_card(board_s *board_config, struct position pos);

int is_neighbour(board_s *board_config, struct position pos, enum vector v);

int has_exit_to(board_s *board_config, struct position pos, struct position target_pos);

int is_next_to_in_the_same_path(board_s *board_config, struct position pos, enum vector v);

void free_board(location **board);

unsigned int pathset_size(board_s *board_config, struct position pos);

location *pathset_representative(board_s *board_config, struct position pos);

/*
 * File board_initialization.c
 */


board_s *new_board_config(unsigned int width,
			  unsigned int height,
			  struct position start,
			  size_t n_objectives,
			  struct position const objectives[],
			  size_t n_holes,
			  struct position const holes[]);


void initialize_board(board_s *board_config,
		      unsigned int width,
		      unsigned int height,
		      struct position start,
		      size_t n_objectives,
		      struct position const objectives[],
		      size_t n_holes,
		      struct position const holes[]);


void set_location_type(board_s *board_config, struct position pos, enum location_type type);


location **make_board(unsigned int width, unsigned int height);

void new_empty_location(location *loc);


void initialize_start(board_s *board_config, struct position start);

void initialize_objectives(board_s *board_config, size_t n_objectives, struct position const objectives[]);

void initialize_holes(board_s *board_config, size_t n_holes, struct position const holes[]);



/*
 *  File board_free.c
 */

void free_board_config(board_s *board_config);

void free_server_board_config(board_s *board_config);


/*
 *  File board_update.c
 */

void update_board(board_s *board_config, struct move const previous_moves[], size_t n_moves);

void update_position_on_path(board_s *board_config, struct position pos);

void put_card(board_s *board_config, struct position pos, enum card_id card, enum direction direction);

void update_pathset_boulder(board_s *board_config, struct position pos);

void spread_representative(board_s *board_config, struct position pos, location *new_representative);

void update_pathset_new_card(board_s *board_config, struct position pos);

void change_pathset_representative(board_s *board_config, struct position pos, location *new_representative);


/*
 * File board_distance.c
 */


unsigned int nb_cards_to_link(board_s *board_config, struct position pos_1, struct position pos_2);

unsigned int distance_between_paths(board_s *board_config, struct position pos_1, struct position pos_2, struct position *out_1, struct position *out_2);

unsigned int path_exits(board_s *board_config, struct position pos, unsigned int max_nb_exits, struct position exits[]);


/*
 * File board_play.c
 */

int can_put_card_here(board_s *board_config, struct position pos, enum card_id card, enum direction dir);

int is_linked_to_start(board_s *board_config, struct position pos);

unsigned int possible_moves_for_card(board_s *board_config, struct position pos[], enum direction dir[], unsigned int nb, enum card_id card);



/*
 * File board_guess.c
 */


void add_guess(board_s *board_config, enum card_id card, struct position pos, enum direction dir);
  
void undo_last_guess(board_s *board_config);

void undo_all_guess(board_s *board_config);


/*
 * File board_print.c
 */


void print_normal_card(enum card_id card);

void print_reversed_card(enum card_id card);

void print_card(enum card_id card, enum direction direction);

void print_board(board_s *board_config);



#endif //SAPOTACHE_BOARD_FUNCTIONS_H
