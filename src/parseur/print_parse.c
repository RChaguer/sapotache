#include "parseur.h"

int	count_card_type(unsigned int card_frequency[])
{
	unsigned int	i;
	unsigned int	count;

	count = 0;
	for (i = 0; i < NB_CARD_ID; i++)
	{
		if (card_frequency[i] != 0)
			count++;
	}
	return (count);
}

void print_positions_of_objectives(t_config *config)
{
	size_t	i;

	for (i  = 0; i < config->n_objectives; i++)
	{
		printf("(%d,%d) ", (config->objectives)[i].x, (config->objectives)[i].y);
	}
}


void print_positions_of_holes(t_config *config)
{
	size_t	i;

	for (i  = 0; i < config->n_holes; i++)
	{
		printf("(%d,%d) ", (config->holes)[i].x, (config->holes)[i].y);
	}
}


void print_board_config(t_config *config){

  printf("Configuration : %ux%u\n",config->height,config->width);
  printf("Card types    : %u\n",count_card_type(config->card_frequency));
  printf("Nb of cards   : %u\n", config->n_cards_total);
  printf("Objectives    : %zu ",config->n_objectives);
  print_positions_of_objectives(config);
  printf("\n"); 

  printf("Holes         : %zu ",config->n_holes);
  print_positions_of_holes(config);
  printf("\n"); 

  printf("Allow boulder : %s\n",((config->allow_boulder) ? "yes" : "no"));
  printf("Allow breaks  : %s\n",((config->allow_breaks) ? "yes" : "no"));
  printf("Repair=Break  : %s\n",((config->repair_eq_break) ? "yes" : "no"));
}


/*
  Usage function
*/
void usage(char * execname) {
  fprintf(stderr, "Usage: %s <filename>\n", execname);
  fprintf(stderr, "   where <filename> is the file to parse\n");
}


/* 
 * Main function 
 */
int main(int argc, char * argv[]) {

  t_config **config;
  int	err;
 
  FILE *f = NULL ;
  *config = NULL; 
  if (argc != 2){
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  f = fopen(argv[1], "r");

  if (f == NULL) {
    printf("*** %s: cannot open file %s\n", argv[0], argv[1]);
    return EXIT_FAILURE;
  }

  err = parse(f, config);
  if (err != SUCCESS)
	return (EXIT_FAILURE);
     print_board_config(*config);
  free(((*config)->objectives));
  free(((*config)->holes));
  free(*config);
  fclose(f);

  return (EXIT_SUCCESS);
}
