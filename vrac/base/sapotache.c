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
void lexer(FILE * f, struct token * tok) {

}


/*
  Parse a Sapotache file
  PRE: f has the expected structure
*/
void parse(FILE * f) {

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
