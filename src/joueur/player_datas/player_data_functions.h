#ifndef SAPOTACHE_PLAYER_DATA_FUNCTIONS_H
#define SAPOTACHE_PLAYER_DATA_FUNCTIONS_H

#include "player_data_structures.h"


/*
 * File player_data_initialisation.c
 */

void initialize_play_data(play_data *data,
			  unsigned int id,
			  enum p_type type, 
			  unsigned int n_cards_total,
			  size_t n_player_cards,
			  enum card_id const player_cards[],
			  unsigned int n_players);

void initialize_player(player_information *player);

void init_player_name(char *s, unsigned int size, unsigned int player_id);

void free_player_data(play_data *data);




/*
 * File player_data_update.c
 */

void remove_card(play_data *data, enum card_id card);

void add_card(play_data *data, enum card_id card);

void update_play_data(play_data *data, struct move const previous_moves[], size_t n_moves);

void update_break_tool(play_data *data, enum card_id card, unsigned int targeted_player);

void update_repair_tool(play_data *data, enum card_id card, unsigned int targeted_player);



/*
 * File player_data_print_status.c
 */

char *str_of_card_id(enum card_id card);

void print_status(play_data *data);



#endif //SAPOTACHE_PLAYER_DATA_FUNCTIONS_H
