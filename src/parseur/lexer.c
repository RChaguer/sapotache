#include "parseur.h"

/*
  skip_commment
  PRE : *c is a # 
  POST : *c is the last character skipped : the first \n or EOF found in the file f
*/
int	skip_comment(FILE *f, char *c)
{
	if (*c != '#')
    		return (ERROR_PARSE_COMMENT);
  	while (*c != '\n' && *c != EOF)
    		*c = fgetc(f);
	return (SUCCESS);
}

int	is_a_number(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_a_space(char c)
{
  return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v');
}

/*
  PRE : *c is a space, \n, \t...
  POST : *c is the first next chararacter which is not a space, \n, \t...
 */
void	skip_space(FILE *f, char *c)
{
	while (is_a_space(*c) && *c != EOF)
	{
 	   	*c = fgetc(f);
  	}
}



/*
  PRE : *c respresents a digit in the file
 Reads a number and fills the tok's str field and type field
 */
int 		token_number(FILE *f, struct token *tok, char *c)
{
	size_t	i = 0;

	tok->type = NUMBER;
	while (i < TOKEN_SIZE && is_a_number(*c))
	{
		(tok->str)[i] = *c;
      		i++;
		*c = fgetc(f);		
  	}
  	(tok->str)[i] = 0;
  	if (!(is_a_space(*c) || (*c == '#')))
		return (ERROR_PARSE_NOT_TOKEN_NUMBER);
	return (SUCCESS);  
}


/*
  PRE : *c respresents a letter in the file
 Reads a card ID and fills the tok's str field and type field
 */
int		token_alpha(FILE *f, struct token *tok, char *c)
{	
	size_t	i = 0;

	tok->type = ID;
	while (i < TOKEN_SIZE && (*c == '_' || (*c >= 'A' && *c <= 'Z')))
	{
    		(tok->str)[i] = *c;
    		i++;
		*c = fgetc(f);		
	}
	(tok->str)[i] = 0;
	if (!(is_a_space(*c) || (*c == '#')))
    		return (ERROR_PARSE_NOT_TOKEN_ALPHA);
	return (SUCCESS);  
}


/*
  Lexer
  POST: tok contains the next token from stream f. The token
  value (str) is truncated at TOKEN_SIZE-1 bytes.
*/
int			lexer(FILE * f, struct token * tok)
{
  	static char	first_call = 1;
  	static char	c = 0;
 
	if (first_call)
	{
		c = fgetc(f);
		first_call = 0;
  	}
	while (c == '#' || (is_a_space(c)))
	{
    		if (c == '#')
      			skip_comment(f, &c);
		if (is_a_space(c))
      			skip_space(f, &c);
	}
  	if (is_a_number(c))
    		return (token_number(f, tok, &c));
	else if (c >= 'A' && c <= 'Z')
    		return (token_alpha(f, tok, &c));
  	switch (c)
	{
  		case ('*'):
    			(tok->type) = STAR;
    			break;
  		case ('%'):
			(tok->type) = PERCENT;
    			break; 
  		case ('>'):
    			(tok->type) = ARROW;
    			break; 
  		case ('$'):
    			(tok->type) = DOLLAR;
    			break;
  		case (EOF):
    			(tok->type) = END_OF_FILE;
    			break;
    		default:
    			return (ERROR_PARSE_NOT_A_TOKEN);
	}
  	c = fgetc(f);
	return (SUCCESS);
}
