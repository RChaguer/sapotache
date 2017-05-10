#include "serveur.h"
#include "../error/error.h"
#include "./board/board_interface.h"


/**  **  **  **  ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/
/* NOTE : For adding a card :                                                          */
/*     if the action type of the card exist already :                                  */
/*         - modify the matching "check_type_card()" fonction in the CHECK MOVE part   */
/*         - modify the matching "put_type_card()" fonction in the DO MOVE part        */
/*     else                                                                            */
/*         - add a case in check_play() with the new type, with the                    */
/*           matching "check_type_card" fonction in the CHECK MOVE part                */
/*         - add a case in check_play() with the new type, with the                    */
/*           matching "put_type_card()" fonction in the DO MOVE part                   */
/*                                                                                     */
/* NOTE : The fonctions must return an enum error type                                 */
/*                                                                                     */
/**  **  **  **  ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/


/** ** ** CHECK MOVE ** ** **/

int check_break_card(enum card_id card, t_player player){
  if(card == CARD_B_AXE && !(player.a_cards[AXE]))
    return SUCCESS;
  else if(card == CARD_B_LAMP && !(player.a_cards[LAMP]))
    return SUCCESS;
  else if(card == CARD_B_CART && !(player.a_cards[CART]))
    return SUCCESS;
  return ERROR;
}

int check_repair_card(enum card_id card, t_player player){
  if (card == CARD_R_AXE && (player.a_cards[AXE]))
    return SUCCESS;
  else if (card == CARD_R_LAMP && (player.a_cards[LAMP]))
    return SUCCESS;
  else if (card == CARD_R_CART && (player.a_cards[CART]))
    return SUCCESS;
  else if (card == CARD_R_ALL && ( (player.a_cards[CART])|| (player.a_cards[LAMP]) || (player.a_cards[AXE]) ))
    return SUCCESS;
  return ERROR;
}

int check_play(struct move c_move, t_game *game ){
  int err; //error variable
  int card;

	//check if the player have the card
	int have_card = 0;
	for(card = 0; card < game->players[c_move.player].nb_card_hand; card++){
	  if(card == c_move.card)
	    have_card++;
	}
	if(have_card == 0)
	  return ERROR_PLAY;

	//check the move or the action
	switch(c_move.act){
		case(ADD_PATH_CARD):
			err = can_put_card_here(game->board, c_move.onto, c_move.card, c_move.dir);
			if(err == 1)
			  err = SUCCESS;
			else
			  err = ERROR;//change the ERROR
			break;
		case(PLAY_BREAK_CARD):
		  err = check_break_card(c_move.card, game->players[c_move.player]); //check if the targeted player have the matching card
		  break;
	        case(PLAY_REPAIR_CARD):
		  err = check_repair_card(c_move.card, game->players[c_move.player]); //check if the targeted player have the matching card
		  break;
	        case(PLAY_BOULDER_CARD):
		  err = there_is_path_card(game->board, c_move.onto); // check if there is a path card on the targeted position  
		  break;
		case(DISCARD):
		  err = SUCCESS;
		  break;
		default:
		  err = ERROR;
		  break;
	}
	return err;
}

/** ** ** DO MOVE ** ** **/


int put_path_card(t_game *game, enum card_id card, struct position onto, enum direction dir){
  put_card(game->board, onto, card, dir);
  return SUCCESS;
}

int put_break_card(enum card_id card, t_player *onplayer){
  onplayer->a_cards[card-CARD_B_AXE]= 1;
  return SUCCESS;
}

int put_repair_card(enum card_id card, t_player *onplayer){
  if (card != CARD_R_ALL){
    onplayer->a_cards[card-CARD_R_AXE]= 0;
  }
  else {
    for(int i = 0; onplayer->a_cards[i] < 3; i++)
      onplayer->a_cards[i]= 0;
  }
  return SUCCESS;
}

int put_boulder_card(t_game *game, struct position onto){
  put_card(game->board, onto, NO_CARD, NORMAL);
  return SUCCESS;
}

int discard(enum card_id card, t_player *player){
  int i = 0;
  while(player->hand[i] != card)
    i++;
  player->hand[i] = NO_CARD;
  return SUCCESS;
}


int check_victory(t_game *game, struct move c_move)
{
  size_t objective = 0;
  int is_next_to = 0;
  struct position tmp_pos;
  for( objective=0; objective < game->config->n_objectives; objective++){ //browse objectives 
    if(game->objectives_status[objective] == HIDDEN){
      tmp_pos = game->config->objectives[objective];
      tmp_pos.x--;
      if(has_exit_to(game->board, tmp_pos, game->config->objectives[objective]))
	is_next_to = 1;
      tmp_pos.x++;
      tmp_pos.y++;
      if(has_exit_to(game->board, tmp_pos, game->config->objectives[objective]))
	is_next_to = 1;
      tmp_pos.x++;
      tmp_pos.y--;
      if(has_exit_to(game->board, tmp_pos, game->config->objectives[objective]))
        is_next_to = 1;
      tmp_pos.x--;
      tmp_pos.y--;
      if(has_exit_to(game->board, tmp_pos, game->config->objectives[objective]))
        is_next_to = 1;
      if(is_next_to){
	if(is_linked_to_start(game->board, game->config->objectives[objective])){
	  if(objective == game->gold_index){
	    game->objectives_status[objective] = GOLD;
	    for (int p = 0; p < game->nb_player; p++)
	      (*(game->players[p].objective_revealed))(game->config->objectives[objective], 1);
	    return VICTORY;
	  }
	  else{
	    game->objectives_status[objective] = REVEALED;
	    for (int p = 0; p < game->nb_player; p++)
	      (*(game->players[p].objective_revealed))(game->config->objectives[objective], 0);
	  }
	}
      }
    }
  }
  return SUCCESS;
}



int do_play(t_game *game, struct move current_move, int nb_move, t_player *current_player){
  int err;
  int i;
  
  //do the move
  switch(current_move.act){
  case(ADD_PATH_CARD):
    err = put_path_card(game,current_move.card,current_move.onto,current_move.dir);
    break;
  case(PLAY_BREAK_CARD):
    err = put_break_card(current_move.card, &(game->players[current_move.player]));
    break;
  case(PLAY_REPAIR_CARD):
    err = put_repair_card(current_move.card, &(game->players[current_move.player]));
    break;
  case(PLAY_BOULDER_CARD):
    err = put_boulder_card(game,current_move.onto);
    break;
  case(DISCARD):
    err = discard(current_move.card, current_player);
    break;
  default:
    err = ERROR;
    break;
  }
  game->previous_move[nb_move % (game->nb_player)] = current_move;
  check_victory(game,current_move);
  return err;
}
