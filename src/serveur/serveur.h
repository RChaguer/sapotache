#ifndef SERVEUR_H
#define SERVEUR_H


#include "../error/error.h"
#include "../parseur/parseur.h"
#include "./board/board_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

enum action_cards {
  AXE=0,        // axe
  LAMP=1,       // lamp
  CART=2,       // cart
};

typedef struct	s_player
{
	char const		*name;
	unsigned int		id_player;
	enum p_type		type;
	enum card_id		hand[6];
	enum action_cards	a_cards[3];
	int                     nb_action_cards;
	int			nb_card_hand;
	int			allow_to_play;
	struct move		(*play)(struct move const previous_moves[], size_t n_moves);
	void			(*draw_card)(enum card_id card);
	void            	(*initialize)(unsigned int id,
		enum p_type type,
		unsigned int width,
		unsigned int height,
		struct position start,
		size_t n_objectives,
		struct position const objectives[],
		size_t n_holes,
		struct position const holes[],
		unsigned int n_cards_total,
		size_t n_player_cards,
		enum card_id const player_cards[],
		unsigned int n_players);
	char const		*(*get_player_name)(void);
	void 			(*objective_revealed)(struct position, int is_gold);
	void			(*finalize)(void);			

}		t_player;

//already define ??
typedef struct s_card{
  enum card_id card;
  enum direction dir;
} t_card;

enum objective_type {
  HIDDEN,
  REVEALED,
  GOLD
};

typedef struct	s_game
{
	
  unsigned int 		nb_player;
  unsigned int gold_index;
  int 			n_cards;
  enum objective_type  *objectives_status;
  struct move		previous_move[10];
  t_player		*players;
  t_config		*config;
  board_s			*board;
  //no field n_move ! (counter in main loop of round)	
}		t_game;



int		str_serveur_error(int err);
int			initialize_game(unsigned int nb_players, t_game *game, FILE *f, FILE **tab_player);
int		card_per_player(unsigned int nb_players);
void					deal_card(t_game *game, unsigned int nb_players);
enum card_id		pick(t_game *game);

int		card_per_player(unsigned int nb_players);
//
//
int check_victory();
int check_path_card();
int check_break_card(enum card_id card, t_player player);
int check_repair_card(enum card_id card, t_player player);
int check_play(struct move c_move, t_game *game );
int put_path_card(t_game *game, enum card_id card, struct position onto, enum direction dir);
int put_break_card(enum card_id card, t_player *onplayer);
int put_repair_card(enum card_id card, t_player *onplayer);
int put_boulder_card(t_game *game, struct position onto);
int discard(enum card_id card, t_player *player);
int do_play(t_game *game, struct move current_move, int nb_move, t_player *current_player);
void	free_serveur(int err, t_game *game, FILE **tab_player, int nb_open);

//
int check_play(struct move c_move, t_game *game);

#endif



