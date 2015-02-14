#include "threest.h"

void load_file(interpreter * myInter) {
	//take in string
	//open file
	//read file in as dictionary
}

//get unix time

void init_linux_builtin(interpreter * myInter) {
	_word tmp;


	tmp.command = "[";
	tmp.builtin = true;
	tmp._word_func = &load_file;
	myInter->add_word(tmp);

}

