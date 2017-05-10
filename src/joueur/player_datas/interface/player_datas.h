#ifndef SAPOTACHE_PLAYER_DATA_INTERFACE_H
#define SAPOTACHE_PLAYER_DATA_INTERFACE_H

#include "../../../interface/interface.h"



void player_datas__initialize(unsigned int id,
			      enum p_type type, 
			      unsigned int n_cards_total,
			      size_t n_player_cards,
			      enum card_id const player_cards[],
			      unsigned int n_players);


void player_datas__remove_card(enum card_id card);

void player_datas__add_card(enum card_id card);

void player_datas__update(struct move const previous_moves[], size_t n_moves);

void player_datas__free();

char const *player_datas__name();

unsigned int player_datas__n_players();

int player_datas__has_the_card(enum card_id card);

unsigned int player_datas__owned_cards(enum card_id cards[], unsigned int nb);

int player_datas__broken_axe_current();

int player_datas__broken_lamp_current();

int player_datas__broken_cart_current();

int player_datas__broken_axe_others(unsigned int id);

int player_datas__broken_lamp_others(unsigned int id);

int player_datas__broken_cart_others(unsigned int id);

unsigned int player_datas__current_id();

void player_datas__status();

#endif //SAPOTACHE_PLAYER_DATA_INTERFACE_H
