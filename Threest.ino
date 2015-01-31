/* -*- c++ -*- */
#include <threest.h>
#include <interpreter.h>
#include <linked_list.h>
#include <vector.h>
#include <hash_map.h>
#include <MemoryFree.h>

//SUCK IT
#ifndef NULL
#define NULL null
#endif

//i should look up the usual max word size
#define BUFF_SIZE 100

interpreter serialInter;
interpreter internalInter;


void setup() {
  init_builtin(&internalInter);
  init_ardu_builtin(&internalInter);
  Serial.begin(9600);
  while(!Serial);
  
}

void loop() {
  
  Serial.print("free memory: ");
  Serial.print(freeMemory());
  
  Serial.print(">");
  
  char input[BUFF_SIZE];
  int tmp;
  int i = 0;
  while (true) {
    while (Serial.available() == 0) {
      //internalInter.parse_line("LOOP");
      //Serial.println("LOOP");
    }
    tmp = Serial.read();
    if (tmp == '\r') break;
    if (tmp == '\n') break;
    //if (tmp < 32 || tmp == 127) break;
    input[i++] = (char) tmp;
    Serial.print((char) tmp);
  }
  input[i] = '\0';
  serialInter.parse_line(input);
  
  while (serialInter.get_error_count() > 0) {
    Serial.print(serialInter.get_error());
  }
  
  while (serialInter.get_output_count() > 0) {
    Serial.print(serialInter.get_output());
  }
  
  Serial.println(" OK");
  
}
