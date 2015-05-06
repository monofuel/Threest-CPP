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

int error_count = 0;

void display_output(const char * line) {
  Serial.print(line);
}

void display_error(const char * line) {
  Serial.print(line);
  error_count++;
}

void output_stub(const char * line) {
  
}

void error_stub(const char * line) {
  error_count++;
}

void setup() {
  init_builtin(&internalInter);
  init_ardu_builtin(&internalInter);
  
  internalInter.add_error = error_stub;
  internalInter.add_output = output_stub;
  serialInter.add_error = display_error;
  serialInter.add_output = display_output;
  
  
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
      internalInter.parse_line("MAIN");
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
  
  Serial.println(" OK");
  
}
