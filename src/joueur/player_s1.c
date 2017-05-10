
#define SIZE_CARDS 20
#define SIZE_POS 100
#define MAX_DIS 1000000


void repair_tool(struct move *m){

  if (player_datas__broken_axe_current()){
    if (player_datas__has_the_card(CARD_R_AXE)){
      m->act = PLAY_REPAIR_CARD;
      m->card_id = CARD_R_AXE;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD
      m->card_id = CARD_R_ALL;
      return;
    }
  }

  if (player_datas__broken_lamp_current()){
    if (player_datas__has_the_card(CARD_R_LAMP)){
      m->act = PLAY_REPAIR_CARD;
      m->card_id = CARD_R_LAMP;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD;
      m->card_id = CARD_R_ALL;
      return;
    }
  }

  if (player_datas__broken_cart_current()){
    if (player_datas__has_the_card(CARD_R_CART)){
      m->act = PLAY_REPAIR_CARD;
      m->card_id = CARD_R_CART;
      return;
    } else  if (player_datas__has_the_card(CARD_R_ALL)){
      m->act = PLAY_REPAIR_CARD
      m->card_id = CARD_R_ALL;
      return;
    }
  }
}

unsigned int dis_lenghten(unsigned int dis_to_obj[], unsigned int nb_objectives, struct position pos){

  unsigned int s;
  unsigned int l;

  for (l = 0 ; l < nb_objectives ; l++){
    s += board__distance_to_objective(pos, l) - dis_to_obj[l];
  }
  return s;
}

void place_path_card(struct move *m){
  
  struct position pos[SIZE_POS];
  enum card_id cards[SIZE_CARDS];
  enum direction dir[SIZE_CARDS];
  unsigned int nb_cards = player_datas__onwned_cards(cards, SIZE_CARDS);
  unsigned int nb_objectives = board__nb_obj();
  unsigned int dis_to_obj[nb_objectives];
  unsigned int nb_pos;
  unsigned int lengthen;
  unsigned int max_lengthen = 0;
  unsigned int min = MAX_DIS;
  int i;
  int k;
  int l;

  if (has_the_card(CARD_D_END)){
    nb_pos = board__possible_moves(pos, dir, SIZE_POS, cards[i]);
    for (k = 0 ; k < nb_pos ; k++){
      for (l = 0 ; l < nb_objectives ; l++){
	i = board__distance_to_objectives(pos[k], l);
	if (i < min){
	  min = i;
	  m->card = CARD_D_END;
	}
      }
    }
  }

  for (i = 0 ; i < nb_cards ; i++){
    nb_pos = board__possible_moves(pos, dir, SIZE_POS, cards[i]);
    for (k = 0 ; k < nb_pos ; k++){
      for (l = 0 ; l < nb_objectives ; l++){
	dis_to_obj[l] = board__distance_to_objectives(pos[k], l);
      }
      board__guess(cards[i], pos[k], dir[k]);
      lengthen = dis_lengthen(dis_to_obj, nb_objectives, pos[k]);
      if (lengthen > max_lengthen){
	max_lengthen = lengthen;
	m->act = ADD_PATH_CARD;
	m->card = cards[i];
	m->onto = pos[k];
	m->dir = dir[k];
      }
      board__guess_undo_once();
    }
  }
}


enum card card_to_discard(){

  enum card_id cards[16] = {CARD_X_CROSS, CARD_H_CROSS, CARD_V_CROSS, CARD_V_LINE, CARD_H_LINE, CARD_L_TURN, CARD_R_TURN, CARD_R_AXE, CARD_R_LAMP, CARD_R_CART, CARD_B_AXE, CARD_B_LAMP, CARD_B_CART, CARD_R_ALL, CARD_D_END, CARD_BOULDER}
  int i;
  
  for (i = 0 ; i < 16 ; i++){
    if (player_datas__has_the_card(cards[i])){
	return cards[i];
	}
  }
}

void boulder(m){
    int dir = 0;
  
  if (player_datas__has_the_card(BOULDER)){
    while ((dir < 4) && (next_to_position(board__start(), dir) == NO_CARD)){
      dir++;
    }
    if (dir >= 4){
      return;
    }
    m->act = PLAY_BOULDER_CARD;
    m->card = CARD_BOULDER;
    m->onto = next_to_position(board__start(), dir);
  }
}

struct move play(struct move const previous_moves[], size_t n_moves){

  struct move m;
  m.player = player_dats__current_id();
  m.act = FAILED;

  break_tool();
  if (!(m.act == FAILED))
    return (m);

  boulder(&m);
  if (!(m.act == FAILED))
    return (m);

  repair_tool(&m);
  if (!(m.act == FAILED))
    return (m);

  if (!(player_datas__broken_axe_current() || player_datas__broken_lamp_current() || player_datas__broken_cart_current())){
  place_path_card(&m);
  }
  if (!(m.act == FAILED))
    return (m);

  m->act = DISCARD;
  m->card = card_to_discard();
  return (m);
}
