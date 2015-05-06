#include "threest.h"
//#include "interpreter.h"

void ardu_delay(interpreter * myInter) {
  delay(myInter->pop()); 
}

void set_pin_output(interpreter * myInter) {
  pinMode(myInter->pop(),OUTPUT); 
}

void set_pin_input(interpreter * myInter) {
  pinMode(myInter->pop(),INPUT); 
}

void set_pin_high(interpreter * myInter) {
  digitalWrite(myInter->pop(),HIGH); 
}

void set_pin_low(interpreter * myInter) {
  digitalWrite(myInter->pop(),LOW); 
}

void init_ardu_builtin(interpreter * myInter) {
	_word tmp;

	tmp.command = "DELAY";
	tmp.builtin = true;
	tmp._word_func = &ardu_delay;
	myInter->add_word(tmp);

	tmp.command = "PIN_OUTPUT";
	tmp.builtin = true;
	tmp._word_func = &set_pin_output;
	myInter->add_word(tmp);

	tmp.command = "PIN_INPUT";
	tmp.builtin = true;
	tmp._word_func = &set_pin_input;
	myInter->add_word(tmp);

	tmp.command = "PIN_HIGH";
	tmp.builtin = true;
	tmp._word_func = &set_pin_high;
	myInter->add_word(tmp);

	tmp.command = "PIN_LOW";
	tmp.builtin = true;
	tmp._word_func = &set_pin_low;
	myInter->add_word(tmp);

	//loop stub for user to redefine
	tmp.command = "MAIN";
	tmp.builtin = false;
	tmp.crates = new linked_list<crate>();
	myInter->add_word(tmp);

}
