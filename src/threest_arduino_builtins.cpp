#include "threest.h"

void ardu_delay() {
  delay(pop()); 
}

void set_pin_output() {
  pinMode(pop(),OUTPUT); 
}

void set_pin_input() {
  pinMode(pop(),INPUT); 
}

void set_pin_high() {
  digitalWrite(pop(),HIGH); 
}

void set_pin_low() {
  digitalWrite(pop(),LOW); 
}

void init_ardu_builtin() {
	_word tmp;

	tmp.command = "DELAY";
	tmp.builtin = true;
	tmp._word_func = &ardu_delay;
	add_word(tmp);

	tmp.command = "PIN_OUTPUT";
	tmp.builtin = true;
	tmp._word_func = &set_pin_output;
	add_word(tmp);

	tmp.command = "PIN_INPUT";
	tmp.builtin = true;
	tmp._word_func = &set_pin_input;
	add_word(tmp);

	tmp.command = "PIN_HIGH";
	tmp.builtin = true;
	tmp._word_func = &set_pin_high;
	add_word(tmp);

	tmp.command = "PIN_LOW";
	tmp.builtin = true;
	tmp._word_func = &set_pin_low;
	add_word(tmp);

	//loop stub for user to redefine
	tmp.command = "LOOP";
	tmp.builtin = false;
	tmp.words = new linked_list<const char *>();
	add_word(tmp);

}
