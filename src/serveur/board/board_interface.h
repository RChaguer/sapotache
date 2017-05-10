#ifndef SAPOTACHE_SERVER_BOARD_INTERFACE_H
#define SAPOTACHE_SERVER_BOARD_INTERFACE_H

#include "../../interface/interface.h"

struct board_s;
typedef struct board_s board_s;


/*
  Allocates a new board configuration and returns its pointer
*/
board_s *new_board_config(unsigned int width,
			  unsigned int height,
			  struct position start,
			  size_t n_objectives,
			  struct position const objectives[],
			  size_t n_holes,
			  struct position const holes[]);


/*
  Returns 1 if the re is a path cards at position pos, else 0
 */
int there_is_path_card(board_s *board_config, struct position pos);


/*
  PRECOND : "card" is a pathcard id (no boulder or breaking/reparing card or no_card) 
  Determines if the card can be put at the position pos (returns 1, else 0)
 */
int can_put_card_here(board_s *board_config, struct position pos, enum card_id card, enum direction dir);


/*
  PRECOND : Function to use : - if can_put_card_here returns 1 for "card" at pos
                              - if card is a NO_CARD : to play after a boulder
  Updates the board.
 */
void put_card(board_s *board_config, struct position pos, enum card_id card, enum direction direction);


/*
  Returns 1 if there is a path linking the start to the position pos, else 0.
 */
int is_linked_to_start(board_s *board_config, struct position pos);


/*
  Returns 1 if the exits of the card put at position pos lead to target_pos, else 0.
 */
int has_exit_to(board_s *board_config, struct position pos, struct position target_pos);


/*
  Frees the board created by new_board_config
*/
void free_server_board_config(board_s *board_config);



#endif //SAPOTACHE_SERVER_BOARD_INTERFACE_H
