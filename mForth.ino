/* -*- c++ -*- */

//SUCK IT
#ifndef NULL
#define NULL null
#endif

#include "mForth.h"
//i should look up the usual max word size
#define BUFF_SIZE 100


void setup() {
  init_builtin();
  init_ardu_builtin();
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Serial.print(">");
  char input[BUFF_SIZE];
  int tmp;
  int i = 0;
  while (true) {
    while (Serial.available() == 0) {
      //parse_line("LOOP");
    }
    tmp = Serial.read();
    if (tmp == '\r') break;
    if (tmp == '\n') break;
    input[i++] = (char) tmp;
    Serial.print((char) tmp);
  }
  input[i] = '\0';
  parse_line(input);
  Serial.println(" OK");
  
}
