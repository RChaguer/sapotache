enum card_id str_to_card_id(char *s){

  if (strcmp("V_LINE",s) == 0){
    return (V_LINE);
  }
  
  if (strcmp("H_LINE",s) == 0){
    return (H_LINE);
  }

  if (strcmp("V_CROSS",s) == 0){
    return (V_CROSS);
  }

  if (strcmp("H_CROSS",s) == 0){
    return (H_CROSS);
  }

  if (strcmp("X_CROSS",s) == 0){
    return (X_CROSS);
  }

  if (strcmp("L_TURN",s) == 0){
    return (L_TURN);
  }

  if (strcmp("R_TURN",s) == 0){
    return (R_TURN);
  }

  if (strcmp("D_END",s) == 0){
    return (D_END);
  }

  if (strcmp("BOULDER",s) == 0){
    return (BOULDER);
  }

  if (strcmp("B_AXE",s) == 0){
    return (B_AXE);
  }

  if (strcmp("B_LAMP",s) == 0){
    return (B_LAMP);
  }
  
  if (strcmp("B_CART",s) == 0){
    return (B_CART);
  }

  if (strcmp("R_AXE",s) == 0){
    return (R_AXE);
  }

  if (strcmp("R_LAMP",s) == 0){
    return (R_LAMP);
  }
  
  if (strcmp("R_CART",s) == 0){
    return (R_CART);
  }

  if (strcmp("R_ALL",s) == 0){
    return (R_ALL);
  }

  return (NO_CARD);
}
