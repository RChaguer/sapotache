#include "board/interface/player_board.h"
#include "player_datas/interface/player_datas.h"


#define SAME_AS(string) strcmp(command,string) == 0

#define RETURN_IF_MATCH(card_name) if (strcmp(#card_name,s) == 0){return CARD_##card_name ;}					     


enum card_id card_id_of_str(char *s){

  RETURN_IF_MATCH(V_LINE)
    RETURN_IF_MATCH(H_LINE)
    RETURN_IF_MATCH(V_CROSS)
    RETURN_IF_MATCH(H_CROSS)
    RETURN_IF_MATCH(X_CROSS)
    RETURN_IF_MATCH(L_TURN)
    RETURN_IF_MATCH(R_TURN)
    RETURN_IF_MATCH(D_END)
    RETURN_IF_MATCH(BOULDER)
    RETURN_IF_MATCH(B_AXE)
    RETURN_IF_MATCH(B_LAMP)
    RETURN_IF_MATCH(B_CART)
    RETURN_IF_MATCH(R_AXE)
    RETURN_IF_MATCH(R_LAMP)
    RETURN_IF_MATCH(R_CART)
    RETURN_IF_MATCH(R_ALL)

    return (NO_CARD);
}


int is_a_path_card(enum card_id card){
  
  return (card == CARD_V_LINE
	  || card == CARD_H_LINE   
	  || card == CARD_V_CROSS  
	  || card ==  CARD_H_CROSS  
	  || card ==  CARD_X_CROSS  
	  || card ==  CARD_L_TURN
	  || card ==  CARD_R_TURN
	  || card ==  CARD_D_END);
}

int is_a_break_card(enum card_id card){

  return (card == CARD_B_AXE
	  || card == CARD_B_LAMP   
	  || card == CARD_B_CART);
}


int is_a_repair_card(enum card_id card){

  return (card == CARD_R_AXE
	  || card == CARD_R_LAMP   
	  || card == CARD_R_CART
	  || card == CARD_R_ALL);
}


struct move create_play_move(){

  struct move m;
  char command[256];

  while (1){
    
    printf("Card to play : ");
    fgets(command,256,stdin);

    if (command[0] != '\0'){
      command[strlen(command)-1] = '\0';
    }
    
    m.card = card_id_of_str(command);

    if (m.card != NO_CARD){
      break;
    }

    printf("Wrong card, please try again.\n");
  }


  if (is_a_path_card(m.card)){

    m.act = ADD_PATH_CARD;
    
    while (1){
      
      printf("Direction : ");
      fgets(command,256,stdin);
      
      if (command[0] != '\0'){
	command[strlen(command)-1] = '\0';
      }
      
      if (SAME_AS("NORMAL")){
	m.dir = NORMAL;
	break;
      }

      if (SAME_AS("REVERSED")){
	m.dir = REVERSED;
	break;
      }
      
      printf("Wrong direction, please try again.\n");
    }
  }

  if (is_a_path_card(m.card) || m.card == CARD_BOULDER){

    printf("Position x : ");
    fgets(command,256,stdin);

    if (command[0] != '\0'){
      command[strlen(command)-1] = '\0';
    }
    
    m.onto.x = atoi(command);
    printf("Position y : ");
    fgets(command,256,stdin);

    if (command[0] != '\0'){
      command[strlen(command)-1] = '\0';
    }
    
    m.onto.y = atoi(command);
    
  }

  if (is_a_repair_card(m.card) || is_a_break_card(m.card)){

    printf("Targeted player id : ");
    fgets(command,256,stdin);

    if (command[0] != '\0'){
      command[strlen(command)-1] = '\0';
    }

    m.onplayer = atoi(command);
      
  }

  if (m.card == CARD_BOULDER){
    m.act = PLAY_BOULDER_CARD;
  } else if (is_a_repair_card(m.card)){
    m.act = PLAY_REPAIR_CARD;
  } else if (is_a_break_card(m.card)){
    m.act = PLAY_BREAK_CARD;
  }

  return (m);
}




struct move create_discard_move(){

  struct move m;
  char command[256];

  while (1){
    
    printf("Card to discard : ");
    fgets(command,256,stdin);

    if (command[0] != '\0'){
      command[strlen(command)-1] = '\0';
    }
    
    m.card = card_id_of_str(command);

    if (m.card != NO_CARD){
      break;
    }

    printf("Wrong card, please try again.\n");
  }

  m.act = DISCARD;
  
  return (m);
}




struct move play_manual(struct move const previous_moves[], size_t n_moves){

  struct move m;
  char command[256];
  char const *name = get_player_name();
  unsigned int id = player_datas__current_id();
  
  while (1){
    
    printf("%s (player %u) >> ",name,id);

    if (fgets(command,256,stdin) == NULL || command[0] == '\0'){
      continue;
    }
   
    command[strlen(command)-1] = '\0';
    
    if (SAME_AS("board")){
      board__print();
      continue;
    }

    if (SAME_AS("status")){
      player_datas__status();
      continue;
    }

    if (SAME_AS("play")){
      return (create_play_move());
    }

    if (SAME_AS("discard")){
      return (create_discard_move());
    }

    printf("Wrong command, please try again.\n");
    
  }
  
  return (m);
}
