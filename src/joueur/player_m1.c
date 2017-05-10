#include "board/interface/player_board.h"
#include "player_datas/interface/player_datas.h"

#define SIZE_CARDS 20
#define SIZE_POS 100


void repair_tool(struct move *m){

  if (player_datas__broken_axe_current()){
    if (player_datas__has_the_card(CARD_R_AXE)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_AXE;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_ALL;
      return;
    }
  }

  if (player_datas__broken_lamp_current()){
    if (player_datas__has_the_card(CARD_R_LAMP)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_LAMP;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_ALL;
      return;
    }
  }

  if (player_datas__broken_cart_current()){
    if (player_datas__has_the_card(CARD_R_CART)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_CART;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD;
      m->card = CARD_R_ALL;
      return;
    }
  }
}

enum card_id card_to_discard(){

  enum card_id cards[16] = {CARD_D_END, CARD_BOULDER, CARD_V_LINE, CARD_H_LINE, CARD_L_TURN, CARD_R_TURN,   CARD_R_AXE, CARD_R_LAMP, CARD_R_CART, CARD_B_AXE, CARD_B_LAMP, CARD_B_CART, CARD_H_CROSS, CARD_V_CROSS, CARD_X_CROSS, CARD_R_ALL};
  int i;
  
  for (i = 0 ; i < 16 ; i++){
    if (player_datas__has_the_card(cards[i]))
      return (cards[i]);
  }

  return (NO_CARD);
}


void break_tool(struct move* m){

  unsigned int curr_p = player_datas__current_id();
  unsigned int nb_p = player_datas__n_players();
  unsigned int p = (curr_p + 1) % nb_p;

  while (p != curr_p){
    if (!(player_datas__broken_axe_others(p) && player_datas__has_the_card(CARD_B_AXE))){
	m->act = PLAY_BREAK_CARD;
	m->card = CARD_B_AXE;
	m->onplayer = p;
      }
    if (!(player_datas__broken_lamp_others(p) && player_datas__has_the_card(CARD_B_LAMP))){
	m->act = PLAY_BREAK_CARD;
	m->card = CARD_B_LAMP;
	m->onplayer = p;
      }
    if (!(player_datas__broken_cart_others(p) && player_datas__has_the_card(CARD_B_CART))){
	m->act = PLAY_BREAK_CARD;
	m->card = CARD_B_CART;
	m->onplayer = p;
      }
    p++;
  }
}



unsigned int dis_shorten(unsigned int dis_to_obj[], unsigned int nb_objectives, struct position pos){

  unsigned int s;
  unsigned int l;

  for (l = 0 ; l < nb_objectives ; l++){
    s += dis_to_obj[l] - board__distance_to_objective(pos, l);
  }
  return s;
}

void place_path_card(struct move *m){
  
  struct position pos[SIZE_POS];
  enum card_id cards[SIZE_CARDS];
  enum direction dir[SIZE_CARDS];
  unsigned int nb_cards = player_datas__owned_cards(cards, SIZE_CARDS);
  unsigned int nb_objectives = board__nb_obj();
  unsigned int dis_to_obj[nb_objectives];
  unsigned int nb_pos;
  unsigned int shorten;
  unsigned int max_shorten = 0;
  int i;
  int k;
  int l;

  for (i = 0 ; i < nb_cards ; i++){
    nb_pos = board__possible_moves_for_card(pos, dir, SIZE_POS, cards[i]);
    for (k = 0 ; k < nb_pos ; k++){
      for (l = 0 ; l < nb_objectives ; l++){
	dis_to_obj[l] = board__distance_to_objective(pos[k], l);
      }
      board__guess(cards[i], pos[k], dir[k]);
      shorten = dis_shorten(dis_to_obj, nb_objectives, pos[k]);
      if (shorten > max_shorten){
	max_shorten = shorten;
	m->act = ADD_PATH_CARD;
	m->card = cards[i];
	m->onto = pos[k];
	m->dir = dir[k];
      }
      board__guess_undo_once();
    }
  }
}


struct move play_m1(struct move const previous_moves[], size_t n_moves){

  struct move m;
  m.player = player_datas__current_id();
  m.act = FAILED;

  repair_tool(&m);
  if (!(m.act == FAILED))
    return (m);

  if (!(player_datas__broken_axe_current() || player_datas__broken_lamp_current() || player_datas__broken_cart_current())){
    place_path_card(&m);
  }
  if (!(m.act == FAILED))
    return (m);
  
  break_tool(&m);
  if (!(m.act == FAILED))
    return (m);

  m.act = DISCARD;
  m.card = card_to_discard();
  return (m);
}
