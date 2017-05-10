#include "parseur.h"


/*
  Changes the value on the config board at position (x,y) to "type" (eg. board(x,y) <- POS_HOLE)  
 */
int	add_position_type(t_config *config, unsigned int x, unsigned int y, enum token_type type)
{
	struct position new;
	static size_t n_malloc_holes = 0;
	static size_t n_malloc_objectives = 0;

	new.x = x;
	new.y = y;
	if (type == PERCENT)
	{
		if (config->n_holes == 0)
		{
			
			config->holes = (struct position *)malloc(5 * sizeof(struct position));
			n_malloc_holes = 5;
			if (config->holes == NULL)
				return (ERROR_MALLOC);
			config->holes[config->n_holes] = new;
			config->n_holes++;
			return (SUCCESS);
		}
		else if (config->n_holes < n_malloc_holes)
		{
			config->holes[config->n_holes] = new;
			config->n_holes++;
			return (SUCCESS);
		}
		else
		{
			config->holes = (struct position *)realloc(config->holes, 2 * n_malloc_holes * sizeof(struct position));
			n_malloc_holes *= 2;
			if (config->holes == NULL)
				return (ERROR_MALLOC);
			config->holes[config->n_holes] = new;
			config->n_holes++;
			return (SUCCESS);
		}	
	}
	else if (type == DOLLAR)
	{
		if (config->n_objectives == 0)
		{
			config->objectives = (struct position *)malloc(5 * sizeof(struct position));
			n_malloc_objectives = 5;
			if (config->objectives == NULL)
				return (ERROR_MALLOC);
			config->objectives[config->n_objectives] = new;
			config->n_objectives++;
			return (SUCCESS);
		}
		else if (config->n_holes < n_malloc_holes)
		{
			config->objectives[config->n_objectives] = new;
			config->n_objectives++;
			return (SUCCESS);
		}
		else
		{
			config->objectives = (struct position *)realloc(config->objectives, 2 * n_malloc_objectives * sizeof(struct position));
			n_malloc_objectives *= 2;
			if (config->objectives == NULL)
				return (ERROR_MALLOC);
			config->objectives[config->n_objectives] = new;
			config->n_objectives++;
			return (SUCCESS);
		}	
	}
	else if (type == ARROW)
	{
		config->start = new;
		return (SUCCESS);
	}
	else
		return (ERROR_PARSE_TYPE);
}



int		parse_board(FILE *f, t_config *config, struct token *current)
{
	int	err;
	int	x;
	int	y;
	int	width = config->width;
	int	height = config->height;
	int	start_met = 0;

	err = lexer(f, current);
	if (err != SUCCESS)
		return (err); 
	if ((current->type != STAR) && (current->type != DOLLAR) && (current->type != ARROW))
		return (ERROR_PARSE_INVALID_BOARD);
	for (y = height-1 ; y >= 0 ; y--)
	{
		for (x = 0; x < width ; x++)
		{
			switch (current->type)
			{
      				case (STAR):
					break;
      				case (PERCENT):
					err = add_position_type(config, x, y, PERCENT);
					if (err != SUCCESS)
						return (ERROR_PARSE);	
					break;

      				case (DOLLAR):
					err = add_position_type(config, x, y, DOLLAR);
					if (err != SUCCESS)
						return (ERROR_PARSE);	
					break; 
      				case (ARROW):
					if(start_met)
						return (ERROR_PARSE);
					start_met = 1;
					err = add_position_type(config, x, y, ARROW);
					if (err != SUCCESS)
						return (ERROR_PARSE);	
					break;
      				default:
					return (ERROR_PARSE);
      			}
			err = lexer(f, current);
			if (err != SUCCESS)
				return (ERROR_PARSE);
    		}
  	}
	return (SUCCESS);
}

int			initialize_empty_config(t_config **config)
{
	*config = (t_config *)malloc(sizeof(t_config));
	if (*config == NULL)
		return (ERROR_MALLOC);
	(*config)->objectives = NULL;
	(*config)->holes = NULL;
	(*config)->n_objectives = 0;
	(*config)->width = 0;
	(*config)->height = 0;
	(*config)->n_cards_total = 0;
	(*config)->n_holes = 0;
	(*config)->allow_boulder = 0;
	(*config)->allow_breaks = 0;
	(*config)->repair_eq_break = 0;
	return (SUCCESS);
}


int			parse_cards(FILE *f, t_config *config, struct token *current)
{
	int		err;
	unsigned int	card_id; 
	unsigned int	card_nb;
  	
	while (current->type != END_OF_FILE)
	{
		if (current->type != ID)
			return (ERROR_PARSE);
      		card_id = str_to_card_id(current->str);    
		if (card_id == NO_CARD || config->card_frequency[card_id] != 0)
			return (ERROR_PARSE);
		err = lexer(f, current);
		if (err != SUCCESS)
			return (err);
		if (current->type != NUMBER)
			return (ERROR_PARSE);		
		if (card_id == CARD_BOULDER)
			config->allow_boulder = 1;
		else if (card_id > CARD_BOULDER)
			config->allow_breaks = 1;
		card_nb = atoi(current->str);
	//	config->card_types++;
		config->card_frequency[card_id] = card_nb;
		config->n_cards_total += card_nb;	    
		err = lexer(f, current);
		if (err != SUCCESS)
			return (ERROR_PARSE);
	}
	return (SUCCESS);
}



/*
  Determines and modifies the field "Reapair=Break" of the config, depending of its set of cards
 */
void	init_repair_eq_break(t_config *config)
{
  int s_axe = config->card_frequency[CARD_R_AXE] - config->card_frequency[CARD_B_AXE];
  int s_lamp = config->card_frequency[CARD_R_LAMP] - config->card_frequency[CARD_B_LAMP];
  int s_cart = config->card_frequency[CARD_R_CART] - config->card_frequency[CARD_B_CART];

  config->repair_eq_break = (s_axe + s_lamp + s_cart + config->card_frequency[CARD_R_ALL] == 0);  
}


/*
  Parse a Sapotache file
  PRE: f has the expected structure
  POST : the board config is initialised, it also prints the results 
*/

int			parse(FILE * f, t_config **config)
{
	struct token 	current;
	int		err;
	 
  	err = initialize_empty_config(config);	
	if (err != SUCCESS)
		return (err);
	//board size
	err = lexer(f, &current);
  	if (err != SUCCESS)
		return (ERROR_PARSE_WRONG_FIRST_NUMBER);
	(*config)->width = atoi(current.str);
	err = lexer(f, &current); 
  	if (err != SUCCESS)
		return (ERROR_PARSE_WRONG_SECOND_NUMBER);
	(*config)->height = atoi(current.str);	
	//board
  	err = parse_board(f, *config, &current);
	if (err != SUCCESS)
		return (err);
  	//cards
  	err = parse_cards(f, *config, &current);
	if (err != SUCCESS)
		return (err);
  	init_repair_eq_break(*config); 
	//print_board_config(config);
  	return (SUCCESS);
}
