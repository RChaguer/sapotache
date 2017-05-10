#ifndef PARSEUR_H
#define PARSEUR_H

#include "../error/error.h"
#include "../interface/interface.h"
#include <stdio.h>
#include <string.h>

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
#define NB_CARD_ID 17
struct token {
  enum token_type type;
  char str[TOKEN_SIZE];
};

typedef struct			s_config
{
	unsigned int		width;
	unsigned int		height;
	struct position		start;
	size_t 			n_objectives;
	struct position		*objectives;
	size_t 			n_holes;
	struct position		*holes;
	unsigned int 		n_cards_total;
//	size_t 			n_player_cards;
//	enum card_id const 	player_cards[];
	unsigned int 		card_frequency[NB_CARD_ID];
	unsigned int		allow_boulder;
	unsigned int 		allow_breaks;
	unsigned int		repair_eq_break;
}				t_config;

///

unsigned int		str_to_card_id(char *s);
int			parse(FILE *, t_config **);
///LEXER_FUNCTIONS
int			skip_comment(FILE *f, char *c);
int			is_a_number(char c);
int			is_a_space(char c);
int			token_alpha(FILE *f, struct token *tok, char *c);
int			token_number(FILE *f, struct token *tok, char *c);
int			lexer(FILE * f, struct token * tok);
int			parse(FILE * f, t_config **config);
#endif
