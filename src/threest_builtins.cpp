#include "threest.h"
#include <stdlib.h>

void create_word(interpreter * myInter) {
	//LOTS OF BIG STUFF
	vector<crate> list = *myInter->get_line();
	int index = myInter->get_current_word();
    int start_index = index;
    _word * my_word = (_word *) malloc(sizeof(_word)); //TODO replace with smart pointer
	crate cur_element = list[++index];
	char * word_copy = (char *) malloc(sizeof(char) * (strlen(cur_element.string_content) + 1));

	strcpy(word_copy,cur_element.string_content);
	my_word->command = word_copy;
	my_word->builtin = false;
	my_word->crates = new linked_list<crate>();

	cur_element = list[++index];
	while (strcmp(cur_element.string_content,";") != 0) {

		
		//INT
		//
		if (isdigit(cur_element.string_content[0])) {
			crate number;
			number.type = INTEGER;
			number.int_content = atoi(cur_element.string_content);
			my_word->crates->push(number);
			cur_element = list[++index];
			continue;
		}
		
		//IF
		//
		if (strcmp(cur_element.string_content,"IF") == 0) {
			crate if_crate;
			if_crate.type = IF;
			if_crate.else_content.then_ptr = -1; //DEFAULT VALUE FOR NO ELSE STATEMENT
			//find our matching ELSE and THEN
			int forward_index = index;
			int if_count = 0;
			do {
				forward_index++;
				if (list[forward_index].type != STRING) continue;

				//if we find another IF word, increment
				//the then_count.
				if (strcmp(list[forward_index].string_content,"IF") == 0) {
					if_count++;
				}
				
				//if we find THEN and the if_count is > 0, 
				//skip it and decrement if_count.
				//otherwise, let's store this index.
				if (strcmp(list[forward_index].string_content,"THEN") == 0) {
					if (if_count == 0) {
                        if_crate.if_content.then_ptr = forward_index - start_index;
						break;
					} else {
						if_count--;
					}
				}
				//if we find ELSE and the if_count is > 0, 
				//let's store this index.
				if (strcmp(list[forward_index].string_content,"ELSE") == 0) {
					if (if_count == 0) {
                        if_crate.if_content.else_ptr = forward_index - start_index;
						break;
					}
				}

			} while (forward_index < list.size());
			if (forward_index == list.size()) {
				myInter->add_error("found IF without matching THEN\n");
			}
			

			my_word->crates->push(if_crate);
			cur_element = list[++index];
			continue;
		}
		//ELSE
		//
		if (strcmp(cur_element.string_content,"ELSE") == 0) {
			crate else_crate;
			else_crate.type = ELSE;
			//find our matching THEN
			int forward_index = index;
			int if_count = 0;
			do {
				forward_index++;
				if (list[forward_index].type != STRING) continue;

				//if we find another IF word, increment
				//the then_count.
				if (strcmp(list[forward_index].string_content,"IF") == 0) {
					if_count++;
				}
				
				//if we find THEN and the if_count is > 0, 
				//skip it and decrement if_count.
				//otherwise, let's store this index.
				if (strcmp(list[forward_index].string_content,"THEN") == 0) {
					if (if_count == 0) {
                        else_crate.else_content.then_ptr = forward_index - start_index;
						break;
					} else {
						if_count--;
					}
				}

			} while (forward_index < list.size());
			if (forward_index == list.size()) {
				myInter->add_error("found ELSE without matching THEN\n");
			}
			

			my_word->crates->push(else_crate);
			cur_element = list[++index];
			continue;
		}
		//THEN
		//
		if (strcmp(cur_element.string_content,"THEN") == 0) {
			crate then_crate;
			then_crate.type = THEN;
			my_word->crates->push(then_crate);

			cur_element = list[++index];
			continue;
		}
		//DO
		//
		if (strcmp(cur_element.string_content,"DO") == 0) {
			crate do_crate;
			do_crate.type = DO;
			my_word->crates->push(do_crate);

			cur_element = list[++index];
			continue;
		}
		//LOOP
		//
		if (strcmp(cur_element.string_content,"LOOP") == 0) {
			crate loop_crate;
			loop_crate.type = LOOP;
			//find our matching DO
			int reverse_index = index;
			int loop_count = 0;
			do {
				reverse_index--;
				if (list[reverse_index].type != STRING) continue;

				//if we find another LOOP word, increment
				//the loop_count.
				if (strcmp(list[reverse_index].string_content,"LOOP") == 0) {
					loop_count++;
				}
				
				//if we find DO and loop count is > 0, 
				//skip it and decrement loop_count.
				//otherwise, let's store this index.
				if (strcmp(list[reverse_index].string_content,"DO") == 0) {
					if (loop_count == 0) {
                        loop_crate.loop_content.do_ptr = reverse_index - start_index;
						break;
					} else {
						loop_count--;
					}
				}

			} while (reverse_index > 0);
			if (reverse_index == 0) {
				myInter->add_error("found LOOP without matching DO\n");
			}
			

			my_word->crates->push(loop_crate);
			cur_element = list[++index];
			continue;
		}
		//FLOAT
		//
		//ARRAY
		//
		//OBJECT
		//
		//STRING
		//
		//BOOL

		//RECURSION
		if (strcmp(cur_element.string_content,"RECURSE") == 0) {
			crate recurse_crate;
			recurse_crate.type = WORD;
			recurse_crate.word_content = my_word;

			my_word->crates->push(recurse_crate);
			cur_element = list[++index];
			continue;
		}
		//WORD
		//check if the word exists in the dictionary
		_word * word_ptr = myInter->get_word(cur_element.string_content);
		if (word_ptr != (_word *) NULL) {
			crate next_word;
			next_word.type = WORD;
			next_word.word_content = word_ptr;
			my_word->crates->push(next_word);
			cur_element = list[++index];
			continue;
		}
		
		
		cur_element = list[++index];
	}

	myInter->add_word(my_word);
	myInter->set_current_word(index);

}

void pop_r(interpreter * myInter) {
	myInter->push(myInter->pop_r());
}

void push_r(interpreter * myInter) {
	myInter->push_r(myInter->pop());
}

void peek_r(interpreter * myInter) {
	myInter->push(myInter->peek_r());
}

void display_1(interpreter * myInter) {
	//TODO do this better
	char * buff = (char *) malloc(50 * sizeof(char));
    snprintf(buff,50,"%d ",myInter->pop());
	myInter->add_output(buff);
    free(buff);
}

void bye(interpreter * myInter) {
	exit(0);
}

void cr(interpreter * myInter) {
    myInter->add_output("\n");
}

void words(interpreter * myInter) {
	/*
	node<_word> * element = myInter.get_global_dict().get_top();
	while (element != NULL) {
        myInter.add_output(element->data.command << " ");
		element = element->next;
	}
	element = myInter.get_local_dict().get_top();
	while (element != NULL) {
        myInter.add_output(element->data.command << " ");
		element = element->next;
    }*/
    //TODO STUB
}

void drop(interpreter * myInter) {
	myInter->pop();
}

void dup(interpreter * myInter) {
	int var = myInter->pop();
	myInter->push(var);
	myInter->push(var);

}

void swap(interpreter * myInter) {

	int var = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var);
	myInter->push(var2);
}

void is_equal(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 == var1);
}

void greater_than(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 > var1);
}

void less_than(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 < var1);
}

void add(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 + var1);
}

void sub(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 - var1);
}

void multiply(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	myInter->push(var2 * var1);
}

void divide(interpreter * myInter) {
	int var1 = myInter->pop();
	int var2 = myInter->pop();
	if (var1 == 0) {
		myInter->add_error("divide by 0 error");
		myInter->push(0);
	} else {
		myInter->push(var2 / var1);
	}
}

void init_builtin(interpreter * myInter) {
	_word tmp;


	tmp.command = ">R";
	tmp.builtin = true;
	tmp._word_func = &push_r;
	myInter->add_word(tmp);

	tmp.command = "R>";
	tmp.builtin = true;
	tmp._word_func = &pop_r;
	myInter->add_word(tmp);

	tmp.command = "I";
	tmp.builtin = true;
	tmp._word_func = &peek_r;
	myInter->add_word(tmp);

	tmp.command = ":";
	tmp.builtin = true;
	tmp._word_func = &create_word;
	myInter->add_word(tmp);

	tmp.command = "DUP";
	tmp.builtin = true;
	tmp._word_func = &dup;
	myInter->add_word(tmp);

	tmp.command = "SWAP";
	tmp.builtin = true;
	tmp._word_func = &swap;
	myInter->add_word(tmp);

	tmp.command = "<";
	tmp.builtin = true;
	tmp._word_func = &less_than;
	myInter->add_word(tmp);

	tmp.command = ">";
	tmp.builtin = true;
	tmp._word_func = &greater_than;
	myInter->add_word(tmp);

    tmp.command = "=";
	tmp.builtin = true;
	tmp._word_func = &is_equal;
	myInter->add_word(tmp);

	tmp.command = "/";
	tmp.builtin = true;
	tmp._word_func = &divide;
	myInter->add_word(tmp);

	tmp.command = "*";
	tmp.builtin = true;
	tmp._word_func = &multiply;
	myInter->add_word(tmp);

	tmp.command = "-";
	tmp.builtin = true;
	tmp._word_func = &sub;
	myInter->add_word(tmp);

	tmp.command = "+";
	tmp.builtin = true;
	tmp._word_func = &add;
	myInter->add_word(tmp);

	tmp.command = ".";
	tmp.builtin = true;
	tmp._word_func = &display_1;
	myInter->add_word(tmp);

	tmp.command = "DROP";
	tmp.builtin = true;
	tmp._word_func = &drop;
	myInter->add_word(tmp);

	tmp.command = "CR";
	tmp.builtin = true;
	tmp._word_func = &cr;
	myInter->add_word(tmp);

	tmp.command = "WORDS";
	tmp.builtin = true;
	tmp._word_func = &words;
	myInter->add_word(tmp);

	tmp.command = "BYE";
	tmp.builtin = true;
	tmp._word_func = &bye;
	myInter->add_word(tmp);
}
