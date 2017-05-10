#ifndef SAPOTACHE_INTERFACE_H
#define SAPOTACHE_INTERFACE_H

#include <stdlib.h>  

/* Positions */
struct position { unsigned int x,y; };

/* Cards */
/* Recall that each card possess a long and a short side. All the
 cards are described here such as the horizontal side is the longest.
 This description will be considered thereafter as the 'normal' direction. */
enum card_id {
  NO_CARD,           // special id for empty card
  CARD_V_LINE,       // vertical line
  CARD_H_LINE,       // horizontal line
  CARD_V_CROSS,      // 3 exits, 2 verticals + 1 left
  CARD_H_CROSS,      // 3 exits, 2 horizontals + 1 up
  CARD_X_CROSS,      // 4 exits
  CARD_L_TURN,       // left turn (down to left)
  CARD_R_TURN,       // right turn (down to right)
  CARD_D_END,        // no exits
  CARD_BOULDER,      // special : boulder card
  CARD_B_AXE,        // break one axe
  CARD_B_LAMP,       // break one lamp
  CARD_B_CART,       // break one cart
  CARD_R_AXE,        // repair one axe
  CARD_R_LAMP,       // repair one lamp
  CARD_R_CART,       // repair one cart
  CARD_R_ALL         // repair anything
};

enum direction {
  NORMAL,            // play card in normal direction
  REVERSED,          // play card reversed
};

/* Action and moves */
enum action {
  ADD_PATH_CARD,     // add path card to the maze
  PLAY_BREAK_CARD,   // play break card
  PLAY_REPAIR_CARD,  // play repair card
  PLAY_BOULDER_CARD, // play "rock-fall" card
  DISCARD,           // discard one card
  FAILED,             // invalid action
};

struct move {
  unsigned int player;   // player issueing the move
  enum action act;       // action taken
  enum card_id card;     // specific card
  struct position onto;  // targeted position on the board
  enum direction dir;    // direction of the card
  unsigned int onplayer; // player targeted by the move
};

/* Player */
enum p_type {
  SAPOTEUR,          // saboteur type
  HONEST,            // gold-objective type
};

/* Public functions */

/* Access to player informations
 * RETURNS:
 * - the player name
 */
char const* get_player_name();

/* Player initialization
 * PARAM:
 * - id: player ID
 * - width, height: dimensions of the board
 * - start: starting position
 * - n_objectives, objectives: number and list of arrival positions
 * - n_holes, holes: number and list of holes on the board
 * - n_cards_total: total number of cards in the game
 * - n_player_cards, player_cards: number and list of cards of the player
 * - n_players: number of players in the game
 * PRECOND:
 * - 0 <= id < n_players
 * - objectives is an array of at least n_objectives elements
 * - holes is an array of at least n_holes elements
 * - player_cards is an array of at least n_player_cards elements
 */
void initialize(unsigned int id,
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

/* Computes next move
 * PARAM:
 * - previous_moves: ordered list of previous moves starting from the last
 *   move issued by the player.
 * - n_moves: number of moves in previous_moves
 * PRECOND:
 * - previous_moves is an array of at least n_moves elements.
 * - previous_moves is an ordered list of previous moves starting from the last
 *   move of the player. Every move invalidated by the server has action
 *   FAILED. Every move validated by the server is faithfully transcribed,
 *   except for the description of actions that may be hidden by the server.
 * RETURNS:
 * - the next move for the player. If the action is ADD_PATH_CARD, PLAY_BREAK_CARD,
 *   PLAY_REPAIR_CARD or PLAY_BOULDER_CARD, then the associated fields inside
 *   the reulting struct move must have been set accordingly :
 *   - for ADD_PATH_CARD     : card, onto and dir
 *   - for PLAY_BREAK_CARD   : card and onplayer
 *   - for PLAY_REPAIR_CARD  : card and onplayer
 *   - for PLAY_BOULDER_CARD : card and onto
 */
struct move play(struct move const previous_moves[], size_t n_moves);

/* Draw card
 * PARAM:
 * - card: the id of the card that has been drawn
 * PRECOND:
 * - this function must be called exactly once directly after a call to
 *   the function "play".
 * - if the card pile in play is empty, card is equal to NO_CARD
 */
void draw_card(enum card_id card);



void objective_revealed(struct position, int is_gold);




/* Cleans up the memory handled by the player at the end of the game
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();

#endif // SAPOTACHE_INTERFACE_H
