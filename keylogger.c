#include <stdio.h>
#include <string.h>
#include "action.h"
#include "kyria.h"

char keylog_str[6] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'z', 'y',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', 'S', '\'', ' ', 'e', '!', ' ',
    ' ', 'e', 'a', ' ', ',', '.', '-', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name[6]={};
  if (keycode == 40) {
    //strcpy(name, "Enter");
    name[0] = 0x1C;
  } else if (keycode == 41) {
    //strcpy(name, "Esc");
    name[0] = 0x0F;
  } else if (keycode == 42) {
    name[0] = 0x1B;
    //strcpy(name, "Back");
  } else if (keycode == 43) {
    name[0] = 0x1A;
    //strcpy(name, "Tab");
  } else if (keycode == 44) {
    strcpy(name, " ");
  } else if (keycode < 60) {
    name[0] = code_to_name[keycode];
  } else if (keycode == 23818) {
    name[0] = 0x19;
    //strcpy(name, "Lower");
  } else if (keycode == 23819) {
    name[0] = 0x18;
    //strcpy(name, "Raise");
  } else if ((keycode == 229) || (keycode == 225)) {
    //strcpy(name, "Shift");
    name[0] = 0x1E;
  } else if (keycode == 224) {
    //strcpy(name, "Ctrl");
    name[0] = 0x01;
  } else if ((keycode == 226) || (keycode == 230)) {
    //strcpy(name, "Alt");
    name[0] = 0x02;
  } else if (keycode == 23825) {
    name[0] = 0x28;
  } else if (keycode == 23826) {
    name[0] = 0x29;
  } else if (keycode == 23821) {
    name[0] = 0x5B;
  } else if (keycode == 23822) {
    name[0] = 0x5D;
  } else if (keycode == 23823) {
    name[0] = 0x7B;
  } else if (keycode == 23824) {
    name[0] = 0x7D;
  } else if (keycode == 100) {
    name[0] = 0x3C;
  } else if (keycode == 23829) {
    name[0] = 0x3E;
  } else if (keycode == 23831) {
    name[0] = 0x2A;
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%1s", name);
  //snprintf(keylog_str, sizeof(keylog_str), "%d", keycode);
}

const char *read_keylog(void) {
  return keylog_str;
}