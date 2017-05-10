#include "../player_data_functions.h"

static play_data data;


void player_datas__initialize(unsigned int id,
			      enum p_type type, 
			      unsigned int n_cards_total,
			      size_t n_player_cards,
			      enum card_id const player_cards[],
			      unsigned int n_players){
  initialize_play_data(&data,id,type,n_cards_total,n_player_cards,player_cards,n_players);
}


void player_datas__remove_card(enum card_id card){
  remove_card(&data,card);
}


void player_datas__add_card(enum card_id card){
  add_card(&data,card);
}


void player_datas__update(struct move const previous_moves[], size_t n_moves){
  update_play_data(&data,previous_moves,n_moves);
}


void player_datas__free(){
  free_player_data(&data);
}


char const *player_datas__name(){
  return (data.name);
}


unsigned int player_datas__n_players(){
  return (data.n_players);
}


int player_datas__has_the_card(enum card_id card){

  unsigned int i;

  for (i = 0 ; i < data.n_player_cards ; i++){

    if (data.player_cards[i] == card){
      return (1);
    }
  }

  return (0);
}


unsigned int player_datas__owned_cards(enum card_id cards[], unsigned int nb){

  unsigned int i;
  unsigned int size = (data.n_player_cards > nb) ? nb : data.n_player_cards;

  for (i = 0 ; i < size ; i++){
    cards[i] = data.player_cards[i];
  }

  return (data.n_player_cards);
}


int player_datas__broken_axe_current(){
  return (data.players[data.id].broken_axe);
}


int player_datas__broken_lamp_current(){
  return (data.players[data.id].broken_lamp);
}


int player_datas__broken_cart_current(){
  return (data.players[data.id].broken_cart);
}


int player_datas__broken_axe_others(unsigned int id){
  return (data.players[id].broken_axe);
}


int player_datas__broken_lamp_others(unsigned int id){
  return (data.players[id].broken_lamp);
}


int player_datas__broken_cart_others(unsigned int id){
  return (data.players[id].broken_cart);
}


unsigned int player_datas__current_id(){
  return (data.id);
}

void player_datas__status(){
  print_status(&data);
}
