#ifndef SAPOTACHE_PLAYER_BOARD_INTERFACE_H
#define SAPOTACHE_PLAYER_BOARD_INTERFACE_H 

#include "../../../interface/interface.h"
#include "../position.h"


void board__initialize(unsigned int width,
		       unsigned int height,
		       struct position start,
		       size_t n_objectives,
		       struct position const objectives[],
		       size_t n_holes,
		       struct position const holes[]);

void board__update(struct move const previous_moves[], size_t n_moves);

void board__free();

int board__invalid_pos(struct position pos);

// Determines if the position next to pos is reachable from pos  
int board__is_next_pos_neighbour(struct position pos, enum vector v);


int board__can_put_card_here(struct position pos, enum card_id card, enum direction dir);


enum card_id board__card_on_pos(struct position pos);


enum direction board__dir_on_pos(struct position pos);


unsigned int board__neighbours_list(struct position pos, struct position neighbours_tab[]);


int board__is_linked_to_start(struct position pos);


int board__is_linked_to_path(struct position pos);

unsigned int board__distance_to_start(struct position pos);

struct position board__start();

struct position *board__obj_list();

unsigned int board__nb_obj();

//return the minimum number of cards to put to reach objective o from a path linked to start
unsigned int board__distance_to_objective(struct position pos, unsigned int o);

//list of the possible positions where card can be played in pos[nb] with the direction in dir[nb], returns the number of positions
unsigned int board__possible_moves_for_card(struct position pos[], enum direction dir[], unsigned int nb, enum card_id card);

//try to play the card card at position pos with direction dir
void board__guess(enum card_id card, struct position pos, enum direction dir);

void board__guess_undo_all();

void board__guess_undo_once();

enum card_id blocked_objective(unsigned int o);


void board__print();

#endif //SAPOTACHE_PLAYER_BOARD_INTERFACE_H 
