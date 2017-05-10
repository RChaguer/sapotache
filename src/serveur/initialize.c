#include "serveur.h"
#include "./board/board_interface.h"

enum card_id		pick(t_game *game)
{
  int				random;
  int				i;
  enum card_id	tmp;

  srand(time(NULL));
  tmp = 0;
  i = 0;
  if (game->config->n_cards_total != 0)
	{
    random = rand()%(game->config->n_cards_total);
    for (tmp = 1; tmp <= CARD_R_ALL; tmp++)
      {
	i += game->config->card_frequency[tmp];
	if (i >= random)
	  {
	    game->config->card_frequency[tmp]--;
	    game->n_cards--; //Changed : index in config remain the same as the begining 
	    return (tmp);
	  }
      }
	return (NO_CARD);
  }
  else 
    return (tmp);
}

void					deal_card(t_game *game, unsigned int nb_players)
{
  int					card_per_players;
  unsigned int		i;
  unsigned int		j;

  i = 0;
  j = 0;
  card_per_players = card_per_player(nb_players);
  for (i = 0; i < nb_players; i++)
    {
      for (j = 0; j < card_per_players; j++)
	game->players[i].hand[j] = pick(game);
      for (j = 0; j < 3; j++)
	game->players[i].a_cards[j] = 0; //init actions cards (table in front of the player)
    }
  return ;
}

int		card_per_player(unsigned int nb_players)
{
  if (nb_players <= 5)
    return (6);
  else if (nb_players <= 7)
    return (5);
  else
    return (4);
}


int					check_card(unsigned int nb_players, t_game *game)
{
  int				nb_path_card;
  enum card_id	card;
	
  nb_path_card = 0;
  for (card = 0; card < CARD_D_END; card++)
    nb_path_card += game->config->card_frequency[card];
  if (nb_path_card < 2*game->config->width)// arbitrary value TODO
    return (ERROR_NOT_ENOUGH_PATH_CARD);
  if (nb_players < 3)
    return (ERROR_NOT_ENOUGH_PLAYER);
  else if (nb_players <= 10)
    return ((nb_players * card_per_player(nb_players) > game->config->n_cards_total) ? ERROR_NOT_ENOUGH_CARD : SUCCESS);
  else
    return (ERROR_TOO_MANY_PLAYER);

}

int compute_nb_sab(int nb_players){
  int max_sab;
  int min_sab;

  if(nb_players <= 4 ){
    max_sab = 1;
    min_sab = 0;
  }
  else if (nb_players <= 6){
    max_sab = 2;
    min_sab = 1;
  }
  else if (nb_players <= 9){
    max_sab = 3;
    min_sab = 2;
  }
  else if (nb_players == 10){
    max_sab = 3;
    min_sab = 3;
  }
  return (rand()% (max_sab-min_sab) + min_sab);
}

void meld_tab(enum p_type tab[], int nb_players)
{
	
  int		random;
  int		i;
  enum p_type	tmp;

  srand(time(NULL));
  for(i = 0; i < nb_players; i++)
    {
      random = rand()%nb_players;
      tmp = tab[random];
      tab[random] = tab[i];
      tab[i] = tmp;	
    }
} 

int			initialize_game(unsigned int nb_players, t_game *game, FILE *f, FILE **tab_player)
{
	int		i; 
	int		card_per_players;
	int		error;
	int 		nb_sap;
	enum p_type	tab[nb_players];


	game->players = NULL;
	game->board = NULL;
	game->config = NULL;
	game->objectives_status = NULL;
	
  	error = parse(f, &(game->config));
	if (game->config == NULL || error != SUCCESS)
		return (error);
	if ((error = check_card(nb_players, game)) != SUCCESS)
		return (error);
	game->n_cards = game->config->n_cards_total;
	game->players = (t_player *)malloc(sizeof(t_player) * nb_players);
	if (game->players == NULL)
		return (ERROR_MALLOC);

	game->nb_player = nb_players;
	game->board = new_board_config(game->config->width,
			  game->config->height,
			  game->config->start,
			  game->config->n_objectives,
			  game->config->objectives,
			  game->config->n_holes,
			  game->config->holes);
	if (game->board == NULL)
		return (ERROR_MALLOC);
	
	game->objectives_status = malloc(sizeof((*(game->objectives_status))) * game->config->n_objectives);//TOFREE
	if (game->objectives_status == NULL)
		return (ERROR_MALLOC);
	for(i = 0; i < game->config->n_objectives; i++)
	  game->objectives_status[i]=0;
	game->gold_index = rand()%game->config->n_objectives;

//deal roles
	nb_sap = compute_nb_sab(nb_players);	
	for (i = 0; i < nb_players; i++){
		if (i < nb_sap)
			tab[i] = SAPOTEUR;
		else
			tab[i] = HONEST;
	}
	meld_tab(tab, nb_players);	
	card_per_players = card_per_player(nb_players);
	for (i = 0; i < nb_players; i++)
	{
		game->players[i].type = tab[i];
		game->players[i].nb_card_hand = card_per_players;
		game->players[i].initialize = dlsym(tab_player[i], "initialize");
		game->players[i].get_player_name =  dlsym(tab_player[i], "get_player_name");
		game->players[i].play =  dlsym(tab_player[i], "play");
		game->players[i].draw_card =  dlsym(tab_player[i], "draw_card");
		game->players[i].finalize =  dlsym(tab_player[i], "finalize");
		game->players[i].objective_revealed =  dlsym(tab_player[i], "objective_revealed");
		if (dlerror() != NULL)
			return (ERROR_DL_SYM);

	}
	return (SUCCESS);
}
