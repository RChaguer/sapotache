#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Definition of tokens constructed by the lexer
*/

enum token_type {
  ID,
  NUMBER,
  STAR,
  ARROW,
  DOLLAR,
  PERCENT,
  END_OF_FILE,
};

#define TOKEN_SIZE 256

struct token {
  enum token_type type;
  char str[TOKEN_SIZE];
};

/*
  Lexer
  POST: tok contains the next token from stream f. The token
  value (str) is truncated at TOKEN_SIZE-1 bytes.
*/

int	token_number(FILE *f, struct token *tok, char *c)
{
	
	size_t	i;

	i = 0;
	tok->type = NUMBER;
	while (i < TOKEN_SIZE && isnum(*c))
	{
		(tok->str)[i] = *c;
		i++;
		*c = fgetc(f);		
	}
	(tok->str)[i] = 0;
	if (!(isspace(c) || (c == '#')))
		return (0);
	return (1);  
}

int	token_alpha(FILE *f, struct token *tok, char *c)
{
	
	size_t	i;

	i = 0;
	tok->type = ID;
	while (i < TOKEN_SIZE && ((*c >= 'A' && *c <= 'Z')))
	{
		(tok->str)[i] = *c;
		i++;
		*c = fgetc(f);		
	}
	(tok->str)[i] = 0;
	if (!(isspace(c) || (c == '#')))
		return (0);
	return (1);  
}



void lexer(FILE * f, struct token * tok) 
{
	static char	c = fgetc(c);

	while (c == '#' || (isspace(c)))
	{
		if (c == '#')	
			skip_comment(f, &c);
		if (isspace(c))
			skip_space(f, &c);
	}	
	if (isnum(c))
	{
		if (!token_number(f, tok, &c))
			tok = NULL;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		if (!token_alpha(f, tok, &c))
				tok = NULL;
	}
	else if (c == '*')
		(tok->type) = STAR;
	else if (c == '%')
		(tok->type) = PERCENT;
	else if (c == '>')
		(tok->type) = ARROW;
	else if (c == '$')
		(tok->type) = DOLLAR;
	else if (c == EOF)
		(tok->type) = END_OF_FILE;
	else
		tok->type = END_OF_FILE ;
	c = fgets(f);
}

/*
  Parse a Sapotache file
  PRE: f has the expected structure
*/

void lex(FILE * f, struct token * tok){
  lexer(f,tok);
  if (tok->type == END_OF_FILE)
    exit_free(); //free cells and error
}
void parse(FILE * f, t_config *config ) 
{
	struct token current;

	//board length

	lex(f,&current);
	if(current.type != NUMBER)
		exit_free();
	config->width = atoi(current.str);
	lex(f,&current);
	if(current.type != NUMBER)
		exit_free();
	config->height = atoi(current.str);
	
	//board

	lex(f,&current);
	if((current.type != STAR) || (current.type != DOLLAR) || (current.type != ARROW) )
		exit_free();
	int x;
	int y;
	config->nb_objectives =0;
	config->objectives =NULL;
	config->nb_holes =0;
	config->holes =NULL;
	
	for(x = width -1 ; y >= 0 ; y--)
	{
		for(y = 0; y < config->length; y++)
		{
		  if(current.type == PERCENT){
		    (config->nb_holes)++;
		    //add_cell(config->holes, x, y);
		  }
		  else if(current.type == DOLLAR){
		    (config->nb_objectives)++;
		    //add_cell(config->holes, x, y);
		  }
		  else if(current.type == ARROW){
		    //if(config->start != NULL)
		    //   exit_free(); //already a starting point
		    //add_cell(config->holes, x, y);
		  }
		  else if(current.type != STAR)
		    exit_free();
		  lex(f,&current);
		}
	}

	//cards

	int card = make_card_number(&current);
	
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
  if (argc != 2) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    printf("*** %s: cannot open file %s\n", argv[0], argv[1]);
    return EXIT_FAILURE;
  }

  parse(f);

  fclose(f);

  return EXIT_SUCCESS;
}
