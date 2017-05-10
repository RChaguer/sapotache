#include "parseur.h"

/*
  Returns the number corresponding to the struct enum card_id for string s, 0 (NO_CARD) if none of the cards match
*/
unsigned int		str_to_card_id(char *s)
{
	unsigned int	i;
	char		*card_id_translation_table[] = { "NO_CARD",
				      "V_LINE",      
				      "H_LINE",       
				      "V_CROSS",      
				      "H_CROSS",      
				      "X_CROSS",      
				      "L_TURN",
				      "R_TURN",
				      "D_END",
				      "BOULDER",
				      "B_AXE", 
				      "B_LAMP",
				      "B_CART",
				      "R_AXE", 
				      "R_LAMP",
				      "R_CART",
				      "R_ALL" };

  	for (i = 0 ; i < NB_CARD_ID ; i++)
	{
    		if (strcmp(card_id_translation_table[i], s) == 0)
      			return (i);
  	}
	return (NO_CARD);
}
