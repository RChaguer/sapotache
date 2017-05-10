#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char location;

#define POS_NOTHING ((location)0)
#define POS_START ((location) 1)
#define POS_HOLE ((location) 2)
#define POS_OBJECTIVE ((location)3)


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



struct board_config {

  unsigned int width;
  unsigned int height;
  
  unsigned int card_types;
  unsigned int card_frequency[NB_CARD_ID];
  unsigned int nb_of_cards;

  unsigned int nb_objectives;
  unsigned int nb_holes;

  location **board;

  int allow_boulder;
  int allow_breaks;
  int repair_eq_break;
};

typedef struct board_config t_config;


char *card_id_translation_table[] = { "NO_CARD",
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
/*
  Returns the number corresponding to the struct enum card_id for string s, 0 (NO_CARD) if none of the cards match
 */
unsigned int str_to_card_id(char *s){

  unsigned int i;

  for (i = 0 ; i < NB_CARD_ID ; i++){
    
    if (strcmp(card_id_translation_table[i], s) == 0){
      return i;
    }
  }
  return (0);
}

/*
  Creates a height x width location matrix and initiallize it to POS_NOTHING.
 */
location **new_board(unsigned int height, unsigned int width){

  unsigned int i;
  unsigned int j;
  location **s = NULL;
  location *data = NULL;

  if (height == 0 || width == 0){
    return NULL;
  }

  s = malloc(height*sizeof(unsigned char*));
  data = malloc(width*height*sizeof(location));
  
  if (s == NULL || data == NULL){
    exit(1);
  }
  
  for (i = 0 ; i < height ; i++){
    s[i] = data + i*width;
  }

  for (i = 0 ; i < height ; i++){

    for (j = 0 ; j < width ; j++){
      s[i][j] = POS_NOTHING;
    }
  }
  return (s);
}

/*
  Changes the value on the config board at location (x,y) to "type" (eg. board(x,y) <- POS_HOLE)  
 */
void add_location_type(t_config *config, unsigned int x, unsigned int y, location type){
  
  config->board[y][x] = type;
}



void free_board(t_config *config){

  if (config->board != NULL){
    
    free(*(config->board)) ;
    free(config->board) ;
    config->board = NULL ;
  }
}


/*
  Initialises an empty board and sets to 0 the number of cards
 */
void init_config_board(t_config *config, unsigned int height, unsigned int width){

  int i ;
  location **m = new_board(height, width);

  if (m == NULL && height != 0 && width != 0){
    fprintf(stderr,"init_config_board : couldn't allocate the board.\n");
    return;
  }

  config->height = height;
  config->width = width;
  
  config->card_types = 0;

  for (i = 0 ; i < NB_CARD_ID ; i++){
    config->card_frequency[i] = 0;
  }
  
  config->nb_of_cards = 0;

  config->nb_objectives = 0;
  config->nb_holes = 0;
  
  config->board = m;

  config->allow_boulder = 0;
  config->allow_breaks = 0;
  config->repair_eq_break = 0;
}


/*
  Determines and modifies the field "Reapair=Break" of the config, depending of its set of cards
 */
void init_repair_eq_break(t_config *config){

  int s_axe = config->card_frequency[CARD_R_AXE] - config->card_frequency[CARD_B_AXE];
  int s_lamp = config->card_frequency[CARD_R_LAMP] - config->card_frequency[CARD_B_LAMP];
  int s_cart = config->card_frequency[CARD_R_CART] - config->card_frequency[CARD_B_CART];

  config->repair_eq_break = (s_axe + s_lamp + s_cart + config->card_frequency[CARD_R_ALL] == 0);  
}



void print_locations_of_type(t_config *config, location type){

  int i;
  int j;
  
  for (i = config->height - 1 ; i >= 0 ; i--){
    
    for (j = config->width - 1 ; j >= 0 ; j--){
      
      if (config->board[i][j] == type){
	printf("(%u,%u) ",j,i);
      }
    }
  }
}


void print_board_config(t_config *config){

  printf("Configuration : %ux%u\n",config->height,config->width);
  printf("Card types    : %u\n",config->card_types);
  printf("Nb of cards   : %u\n",config->nb_of_cards);

  printf("Objectives    : %u ",config->nb_objectives);
  print_locations_of_type(config,POS_OBJECTIVE);
  printf("\n") ; 

  printf("Holes         : %u ",config->nb_holes);
  print_locations_of_type(config,POS_HOLE);
  printf("\n") ; 

  printf("Allow boulder : %s\n",((config->allow_boulder) ? "yes" : "no"));
  printf("Allow breaks  : %s\n",((config->allow_breaks) ? "yes" : "no"));
  printf("Repair=Break  : %s\n",((config->repair_eq_break) ? "yes" : "no"));
}


/*
  skip_commment
  PRE : *c is a # 
  POST : *c is the last character skipped : the first \n or EOF found in the file f
*/
void skip_comment(FILE *f, char *c){

  if (*c != '#'){
    fprintf(stderr,"skip_comment : first character %c is not a #.\n",*c);
    return;
  }
  
  while (*c != '\n' && *c != EOF){
    *c = fgetc(f);
  }
}


int is_a_number(char c){

  return (c >= '0' && c <= '9');
  
}

int is_a_space(char c){

  return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v');
  
}

/*
  PRE : *c is a space, \n, \t...
  POST : *c is the first next chararacter which is not a space, \n, \t...
 */
void skip_space(FILE *f, char *c){

  while (is_a_space(*c) && *c != EOF){
    *c = fgetc(f);
  }
  
}


/*
  PRE : *c respresents a digit in the file
 Reads a number and fills the tok's str field and type field
 */
int token_number(FILE *f, struct token *tok, char *c){

  size_t i = 0;

  tok->type = NUMBER;

  while (i < TOKEN_SIZE && is_a_number(*c)){
      (tok->str)[i] = *c;
      i++;
      *c = fgetc(f);		
  }

  (tok->str)[i] = 0;

  if (!(is_a_space(*c) || (*c == '#'))){
    return (0);
  }

  return (1);  
}


/*
  PRE : *c respresents a letter in the file
 Reads a card ID and fills the tok's str field and type field
 */
int token_alpha(FILE *f, struct token *tok, char *c){
	
  size_t i = 0;

  tok->type = ID;

  while (i < TOKEN_SIZE && (*c == '_' || (*c >= 'A' && *c <= 'Z'))){
    (tok->str)[i] = *c;
    i++;
    *c = fgetc(f);		
  }

  (tok->str)[i] = 0;

  if (!(is_a_space(*c) || (*c == '#'))){
    return (0);
  }

  return (1);  
}


/*
  Error function : frees the board, displays the error message on stderr, and cuts the program
 */
void exit_free(t_config *config, char *error_message){
  
  fprintf(stderr, "Parsing error : %s \n", error_message);

  if (config != NULL){
    free_board(config);
  }
  
  exit(EXIT_FAILURE);
}




/*
  Lexer
  POST: tok contains the next token from stream f. The token
  value (str) is truncated at TOKEN_SIZE-1 bytes.
*/
void lexer(FILE * f, t_config *config, struct token * tok){

  static char first_call = 1;
  static char c = 0;
 
  if (first_call){
    c = fgetc(f);
    first_call = 0;
  };

  while (c == '#' || (is_a_space(c))){

    if (c == '#'){	
      skip_comment(f, &c);
    }

    if (is_a_space(c)){
      skip_space(f, &c);
    }
  }
  
  if (is_a_number(c)){
    
    if (!token_number(f, tok, &c)){
      exit_free(config,"Invalid number format") ;

    } else {
      return ;
    }
  } else if (c >= 'A' && c <= 'Z'){

    if (!token_alpha(f, tok, &c)){
      exit_free(config, "Invalid card ID format");
    } else {
      return ;
    }
  }
  
  switch (c){
    
  case '*':
    (tok->type) = STAR;
    break;
    
  case '%':
    (tok->type) = PERCENT;
    break;
    
  case '>':
    (tok->type) = ARROW;
    break;
    
  case'$':
    (tok->type) = DOLLAR;
    break;

  case EOF:
    (tok->type) = END_OF_FILE;
    break;
    
  default:
    exit_free(config, "Unable to determine next token");
    
  };
  
  c = fgetc(f);
  
}


void parse_board(FILE *f, t_config *config, struct token *current){

  int x;
  int y;
  int width = config->width;
  int height = config->height;
  int start_met = 0;
  
  lexer(f, config, current);
   
  if ((current->type != STAR) && (current->type != DOLLAR) && (current->type != ARROW)){
    exit_free(config,"Invalid board description");
  }

  for (y = height-1 ; y >= 0 ; y--){

    for (x = 0; x < width ; x++){

      switch (current->type){
	    
      case STAR:
	break;

      case PERCENT:
	(config->nb_holes)++;
	add_location_type(config, x, y, POS_HOLE);
	break;

      case DOLLAR:
	(config->nb_objectives)++;
	add_location_type(config, x, y, POS_OBJECTIVE);
	break;
		    
      case ARROW:
	if(start_met){  
	  exit_free(config, "More than one start found");
	}
	start_met = 1;
	add_location_type(config, x, y, POS_START);
	break ;

      default:
	exit_free(config,"Invalid board description");

      } ;

      lexer(f,config,current);	  
    }		
  }
}



void parse_cards(FILE *f, t_config *config, struct token *current){
  
  unsigned int card_id ; 
  unsigned int card_nb ;
  
  while (current->type != END_OF_FILE){

      if (current->type != ID){
	exit_free(config, "Expecting a card ID");
      }
	  
      card_id = str_to_card_id(current->str) ;
	    
      if (card_id == NO_CARD || config->card_frequency[card_id] != 0){
	exit_free(config, "Invalid card ID") ;
      }

      lexer(f, config, current);

      if (current->type != NUMBER){
	  exit_free(config, "Expecting a number after card ID");
      }

      if (card_id == CARD_BOULDER){
	  config->allow_boulder = 1;

      } else if (card_id > CARD_BOULDER){
	config->allow_breaks = 1;
      }
	    
      card_nb = atoi(current->str);
      config->card_types++;
      config->card_frequency[card_id] = card_nb;
      config->nb_of_cards += card_nb;
	    
      lexer(f,config,current) ;
    }
}

/*
  Parse a Sapotache file
  PRE: f has the expected structure
  POST : the board config is initialised, it also prints the results 
*/
void parse(FILE * f, t_config *config){

  struct token current;
  
  init_config_board(config, 0, 0);
  
  //board size
  lexer(f, config, &current);
  
  if(current.type != NUMBER){
    exit_free(config, "First number (board width) not found");
  }

  config->width = atoi(current.str);
  lexer(f, config, &current);
  
  if(current.type != NUMBER){
    exit_free(config, "Second number (board height) not found");
  }

  config->height = atoi(current.str);
	
  //board
  init_config_board(config, config->height, config->width);
  parse_board(f, config, &current);
	
  //cards
  parse_cards(f, config, &current);
  init_repair_eq_break(config);
  
  print_board_config(config);
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

  t_config config ;
  FILE *f = NULL ;

  if (argc != 2){
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  f = fopen(argv[1], "r");

  if (f == NULL) {
    printf("*** %s: cannot open file %s\n", argv[0], argv[1]);
    return EXIT_FAILURE;
  }

  parse(f, &config);
  
  free_board(&config);
  fclose(f);

  return (EXIT_SUCCESS);
}
