#include "serveur.h"
#include "../parseur/parseur.h"

int			round_hajime(t_game *game, FILE **tab_player, int *winner)
{
	int		current_player;
	int		err;
	int		result;
	int		nb_players_not_allowed;
	struct move	current_move;
	enum card_id    card_picked;
	int		current_round;
	int		nb_move;
	int 		i;

	nb_players_not_allowed = 0;
	current_round = 0;
	//game main loop
	while (1)
	{
		current_player = 0;
		while (current_player < game->nb_player) 
		{
			if (game->players[current_player].allow_to_play == 0)
			{
			  nb_move = game->nb_player * current_round + current_player;//
				current_move = (*(game->players[current_player].play))((game->previous_move), (size_t)(nb_move) );

				err = check_play(current_move, game);
				if (err != SUCCESS)
				{
					game->players[current_player].allow_to_play = 1;
					nb_players_not_allowed++;
					current_move.act = FAILED;
				}
				else
				{
				  result = do_play(game, current_move, (nb_move)%(game->nb_player), &(game->players[current_player]));
					if(result == ERROR)
					  return(ERROR_DO_PLAY);

					//round end :
					
					if (result == VICTORY)
					{
					  *winner = current_player;//players won !
							return (SUCCESS);
					}
					if (game->config->n_cards_total != 0)
					{
							card_picked = pick(game);
							(game->players[current_player].draw_card)(card_picked);
							i = 0;
							while(game->players[current_player].hand[i] != NO_CARD)
							  i++;
							game->players[current_player].hand[i] = card_picked;
					}
				}
			}
			current_player++;
		}
		current_round++;
		
		if (nb_players_not_allowed == game->nb_player || game->n_cards == 0)
		{
		  *winner = -1;//sapoters won !
			return (SUCCESS);
		}
	}
}


int			hajime(t_game *game, unsigned int nb_players, FILE *f, FILE **tab_player, int *winner)
{
	int		i;
	int		err;
					      
	err = initialize_game(game->nb_player, game, f, tab_player);
	if (err != SUCCESS)
		return (err);
	deal_card(game, game->nb_player);
	for (i = 0; i < nb_players; i++)
	{	
		(*(game->players[i].initialize))(i,
		game->players[i].type, 
		game->config->width, 
		game->config->height, 
		game->config->start,
		game->config->n_objectives,
		game->config->objectives,
		game->config->n_holes,
		game->config->holes,
		game->n_cards,
		game->players[i].nb_card_hand,
		game->players[i].hand,
		game->nb_player);

		game->players[i].name = (*(game->players[i].get_player_name))();
		game->players[i].allow_to_play = 0;
	}
	err = round_hajime(game, tab_player, winner);
	for (i = 0; i < nb_players; i++)
	{	
		(*(game->players[i].finalize))();
	}

	return (err);
}

int		initialize_player(int argc, char **argv, FILE **tab_player, int *nb_file_open)
{
	FILE	*tmp;
	int	i;

	
	tab_player = (FILE **)malloc((argc - 1) * sizeof(FILE *));
	*nb_file_open = 0;
	if (tab_player == NULL)
		return (ERROR_MALLOC_INITILIAZE_PLAYER);
	for (i = 1; i < argc; i++)
	{
		tmp = (FILE *)dlopen(argv[i], RTLD_LAZY);	
		if (tmp == NULL)
			return (ERROR_OPEN_SO);
		(*nb_file_open)++;
		tab_player[i - 1] = tmp;
		
	}
	return (SUCCESS);		
}

/* 
 * Main function 
 */

int	main(int argc, char **argv)
{
	int	winner;
	int	err;
	int	nb_file_so_open;
	t_game	game;
	FILE	*f;
	FILE	**tab_player;

	tab_player = NULL;
	if ((f = fopen("fichier", "r")) == NULL)
		return (str_serveur_error(ERROR_OPEN_FILE));
	if (argc < 3)
		return (str_serveur_error(ERROR_NOT_ENOUGH_PLAYER));
	err = initialize_player(argc, argv, tab_player, &nb_file_so_open);
	if (err == ERROR_OPEN_SO || err == ERROR_MALLOC_INITILIAZE_PLAYER)
	{
		free_serveur(err, &game, tab_player, nb_file_so_open);
		if((err = fclose(f)) == -1)
			return (str_serveur_error(ERROR_CLOSE_FILE)); 
		return (err);
	}
	err = hajime(&game, argc - 1, f, tab_player, &winner);
	if (err != SUCCESS)
	{
		free_serveur(err, &game, tab_player, nb_file_so_open);
		if((err = fclose(f)) == -1)
			return (str_serveur_error(err));
		return (err);
	}
	if((err = fclose(f)) == -1)
		str_serveur_error(ERROR_CLOSE_FILE); 
	free_serveur(err, &game, tab_player, nb_file_so_open);
	return (SUCCESS);	
}
