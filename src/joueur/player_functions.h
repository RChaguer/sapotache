#ifndef SAPOTACHE_PLAYER_FUNCTIONS
#define SAPOTACHE_PLAYER_FUNCTIONS

#include "../interface/interface.h"

enum vector {
  UP,
  DOWN,
  LEFT,
  RIGHT
} 

/*
  Function to call before anything else when the server calls the function "play"
*/
void update_informations(struct move const previous_moves[], size_t n_moves);

/*
  Positions primitives
 */

// Determines whether the position is inside the board
int is_invalid_pos(struct position pos);

// Returns one of the 4 positions next to pos
struct position next_to_pos(struct position pos, enum vector v);

// Determines if the position next to pos is reachable from pos  
int is_next_pos_neighbour(struct position pos, enum vector v);

// 
int can_put_card_here(struct position pos, enum card_id card, enum direction dir);

// 
enum card_id card_on_pos(struct position pos);

enum direction dir_on_pos(struct position pos);


unsigned int neighbours_list(struct position pos, struct position neighbours_tab[]);


int is_linked_to_start(struct position pos);

int has_the_card(enum card);

int is_linked_to_path(struct position pos);

unsigned int distance_to_start(struct position pos);

struct position* obj_list();

unsigned int nb_obj();

//return the minimum number of cards to put to reach objective o from a path linked to start
unsigned int distance_to_objective(struct position pos, unsigned int o);

//list of the possible positions where card can be played in pos[nb] with the direction in dir[nb], returns the number of positions
unsigned int possible_moves(struct position pos[], enum direction dir[], unsigned int nb, enum card_id card);

//list of the cards owned by current player in cards[nb], returns the number of cards
unsigned int owned_cards(enum card_id cards[], unsigned int nb);

//try to play the card card at position pos with direction dir
void guess (enum card_id card, struct position pos, enum direction dir);

void guess_undo_all();

void guess_undo_once();

unsigned int broken_axe_current();

unsigned int broken_lamp_current();

unsigned int broken_cart_current();

unsigned int broken_axe_others(unsigned int id);

unsigned int broken_lamp_others(unsigned int id);

unsigned int broken_cart_others(unsigned int id);

unsigned int current_id();

enum card_id blocked_objective(unsigned int o);

#endif //SAPOTACHE_PLAYER_FUNCTIONS
