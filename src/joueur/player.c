#include "board/interface/player_board.h"
#include "player_datas/interface/player_datas.h"

#ifndef USED_PLAY_FUNCTION
#define USED_PLAY_FUNCTION play_manual
#endif

struct move USED_PLAY_FUNCTION(struct move const previous_moves[], size_t n_moves);



char const* get_player_name(){
  return player_datas__name();
}

void initialize(unsigned int id,
		enum p_type type,
		unsigned int width,
		unsigned int height,
		struct position start,
		size_t n_objectives,
		struct position const objectives[],
		size_t n_holes,
		struct position const holes[],
		unsigned int n_cards_total,
		size_t n_player_cards,
		enum card_id const player_cards[],
		unsigned int n_players){

  player_datas__initialize(id,type,n_cards_total,n_player_cards,player_cards,n_players);
  board__initialize(width,height,start,n_objectives,objectives,n_holes,holes);

}


struct move play(struct move const previous_moves[], size_t n_moves){

  struct move m ;

  player_datas__update(previous_moves,n_moves);
  board__update(previous_moves,n_moves);

  m = USED_PLAY_FUNCTION(previous_moves,n_moves);

  player_datas__remove_card(m.card);
  
  return (m);
}



void draw_card(enum card_id card){
  player_datas__add_card(card);
}



void objective_revealed(struct position objective_pos, int is_gold){

  struct move m;
  
  if (is_gold){
    return;
  }

  m.act = ADD_PATH_CARD;
  m.card = CARD_X_CROSS;
  m.onto = objective_pos;
  m.dir = NORMAL;
  
  board__update(&m,1);
  
}


void finalize(){
  
  player_datas__free();
  board__free();
  
}
