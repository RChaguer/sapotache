#include "../serveur/serveur.h"


void	free_parse(t_game *game)
{
	if (game->config != NULL)
	{
		if (game->config->objectives != NULL)
			free(game->config->objectives);
		if (game->config->holes != NULL)
			free(game->config->holes);
		free(game->config);
		game->config = NULL;
	}
	return ;
}


void	free_game(t_game *game)
{
	if (game->config != NULL)
		free_parse(game);
	if (game->players != NULL)
		free(game->players);	
	//free_board();
}

void	free_serveur(int err, t_game *game, FILE **tab_player, int nb_open)
{
	int	i;
	
	//Serveur error
	if (err == ERROR_MALLOC_INITILIAZE_PLAYER)
	{		
		str_serveur_error(err);
		return ;
	}
	else if (err == ERROR_OPEN_SO)
	{
		for (i = 0; i < nb_open; i++)			
		{
			dlclose(tab_player[i]);
		}
		free(tab_player);
		str_serveur_error(err);
		return ;
	}
	else
	{
		//Free everything 
		for (i = 0; i < nb_open; i++)			
		{
			dlclose(tab_player[i]);
		}
		free(tab_player);
		free_game(game);			
		str_serveur_error(err);
		return ;
	}
}


int	str_serveur_error(int err)
{
	switch(err)
	{
		case (ERROR_OPEN_FILE):
		fprintf(stdout, "Can't open config file\nUsage : Name of config file :config.sapotache");	
		break;
		
		case (ERROR_CLOSE_FILE):
		fprintf(stderr, "Servor error : Can't close file\n");	
		break;
	
		case (ERROR_MALLOC):
		fprintf(stderr, "Servor error : Not enough memory\n");	
		break;
		
		case (ERROR_MALLOC_INITILIAZE_PLAYER):
		fprintf(stderr, "Servor error : Not enough memory\n");	
		break;
	
		case (ERROR_NOT_ENOUGH_PLAYER):
		fprintf(stdout, "Usage : Not enough players. At least 3 players needed\n");	
		break;
		
		case (ERROR_TOO_MANY_PLAYER):
		fprintf(stdout, "Usage : Too many players. Less than 10 players needed\n");	
		break;
		
		case (ERROR_NOT_ENOUGH_CARD):
		fprintf(stderr, "Parse error : Not enough cards\n");	
		break;
		
		case (ERROR_NOT_ENOUGH_PATH_CARD):
		fprintf(stderr, "Parse error : Not enough cards\n");	
		break;
			
		case (ERROR_OPEN_SO):
		fprintf(stderr, "Servor error : Can't open .so file\n");	
		break;
		
		case (ERROR_PARSE):
		fprintf(stdout, "Usage : Format config.sapotache");
		system("cat projSapotacheFormat.txt");
		break;
	
		case (ERROR_PARSE_WRONG_FIRST_NUMBER):
		fprintf(stderr, "Parse error : Wrong first number\n");	
		system("cat projSapotacheFormat.txt");
		break;
			
		case (ERROR_PARSE_WRONG_SECOND_NUMBER):
		fprintf(stderr, "Parse error : Wrong first number\n");	
		system("cat projSapotacheFormat.txt");
		break;
	
		case (ERROR_PARSE_INVALID_BOARD):
		fprintf(stderr, "Parse error : Invalid board\n");	
		system("cat projSapotacheFormat.txt");
		break;
	
		case (ERROR_DL_SYM):
		fprintf(stderr, "Servor error : Error dlsym\n");	
		break;
		
		case (ERROR_PLAY):
		fprintf(stderr, "Playing error\n");	
		break;
		
		default:
		fprintf(stderr, "Unknown error\n");	
		break;
	}
	return (err);		
}
