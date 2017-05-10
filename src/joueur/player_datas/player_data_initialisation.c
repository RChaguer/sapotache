#include "player_data_functions.h"





void initialize_play_data(play_data *data,
			  unsigned int id,
			  enum p_type type, 
			  unsigned int n_cards_total,
			  size_t n_player_cards,
			  enum card_id const player_cards[],
			  unsigned int n_players){

  int i;
  
  data->id = id;
  data->type = type;

  init_player_name(data->name,MAX_STR_LENGTH,id);
  
  data->n_cards_total = n_cards_total;

  data->n_player_cards = n_player_cards;
  data->player_cards = malloc(n_player_cards*sizeof(enum card_id));

  //ERREUR A GERER
  if (data->player_cards == NULL){
    return ;
  }

  memcpy(data->player_cards,player_cards,n_player_cards*sizeof(enum card_id));
  
  data->n_players = n_players;
  data->players = malloc(n_players*sizeof(struct player_information));

  //ERREUR A GERER
  if (data->players == NULL){
    return;
  }

  for (i = 0 ; i < n_players ; i++){
    initialize_player(data->players + i);
  }

}


void initialize_player(player_information *player){

  player->broken_axe = 0;
  player->broken_cart = 0;
  player->broken_lamp = 0;

}


 
void init_player_name(char *s, unsigned int size, unsigned int player_id){

  char answer[MAX_STR_LENGTH];
  int valid = 0;
  
  do {
    
    printf("Please enter name for player %u : ",player_id);

    if (fgets(s,MAX_STR_LENGTH,stdin) == NULL || s[0] == '\0'){
      printf("Invalid name, try again.\n");
      
    } else {
      
      s[strlen(s)-1] = '\0';
      printf("Are you sure you want your name to be \"%s\" ? (y/n) ",s);

      if (fgets(answer,MAX_STR_LENGTH,stdin) == NULL || (answer[0] != 'y' && answer[0] != 'n')){
	printf("Invalid answer.\n");
      } else if (answer[0] == 'y'){
	valid = 1;
      }
    }
    
  } while (!valid);
}



void free_player_data(play_data *data){

  free(data->player_cards);
  free(data->players);
 
}
