#include "threest.h"
#include "interpreter.h"

linked_list<_word *> interpreter::global_dict;

vector<crate> * interpreter::get_line() {
	return current_line;
}

int interpreter::get_current_word() {
	return current_word;
}

void interpreter::set_current_word(int var) {
	current_word = var;
}

linked_list<_word *> interpreter::get_global_dict() {
    return global_dict;
}

linked_list<_word *> interpreter::get_local_dict() {
    return local_dict;
}

void interpreter::push(int var) {
	stack.push(var);
}

void interpreter::push_r(int var) {
	return_stack.push(var);
}

int interpreter::pop() {
	if (stack.size() == 0) {
		//error case
        add_error("stack underflow\n");
		return 0;
	} else {
		return stack.pop();
	}
}

int interpreter::pop_r() {
	if (return_stack.size() == 0) {
		//error case
        add_error("return stack underflow\n");
		return 0;
	} else {
		return return_stack.pop();
	}
}

int interpreter::peek() {
	if (stack.size() == 0) {
		add_error("stack underflow\n");
		return 0;
	} 
	return stack.get_top()->data;
}

int interpreter::peek_r() {
	if (return_stack.size() == 0) {
		add_error("return stack underflow\n");
		return 0;
	} 
	return return_stack.get_top()->data;
}

void interpreter::add_word(_word item) {
	_word * word_copy = (_word *) malloc(sizeof(_word));

	*word_copy = item;
	add_word(word_copy);


}

void interpreter::add_word(_word * item) {

	//check if it will be global or not
	if (item->command[0] == '#') {
		//local
		node<_word *> * current = local_dict.get_top();
		while (current != NULL) {
			if (strcmp(current->data->command,item->command) == 0) {
				if (current->data->builtin) {
                    add_error("cannot redefine built-in words\n");
					return;
				}
				//free(current->data.crates);
				current->data = item;
			}
			current = current->next;
		}
		local_dict.push(item);
	} else {
		//global
		node<_word *> * current = global_dict.get_top();
		while (current != NULL) {
			if (strcmp(current->data->command,item->command) == 0) {
				if (current->data->builtin) {
                    add_error("cannot redefine built-in words");
					return;
				}
				//free(current->data.crates);
				current->data = item;

			}
			current = current->next;
		}
		global_dict.push(item);
	}
}

void interpreter::run_word(crate item) {
	if (bail_out) return;

	if (return_lines.size() > 20) {
		bail_out = true;
		return;

	}

	int index;
	int limit;
	int choice;
	_word * element;
	//execute the item
	switch (item.type) {
		case DO:
			//set up the return stack
			index = pop();
			limit = pop();
			push_r(limit);
			push_r(index);

			break;
		case LOOP:
			index = pop_r();
			limit = pop_r();
			index++;

			//crude limit so that i don't keep shooting myself in the foot
			if (index >= 2000) {
				add_error("Loop limit exceeded, gun foot shoot prevented\n");
				break;
			}

			if (limit == index) {
				break;
			} else {
				current_word = item.loop_content.do_ptr + 1;
				push_r(limit);
				push_r(index);
			}

			break;

		case IF:
			//pop the value off the stack and see if it's true.
			//if it is, continue execution. otherwise, jump to else.
			//if else doesn't exist, jump to then.
			//the compiler assumes if else is -1, else does not exist
			// since negative values make no sense for our word array
			choice = pop();
			if (!choice) {
				//if it is false, jump to else. if else does not exist, jump to then
				if (item.if_content.else_ptr == -1) {
					//then isn't a real word so we can just skip it
					current_word = item.if_content.then_ptr + 1;
				} else {
					//jump to the word directly after else for execution
					//rather than execute the else word.
					current_word = item.if_content.else_ptr + 1;
				}
			} //else continue execution

			break;
		case ELSE:
			//if we hit ELSE, this means we were executing along the IF TRUE branch
			// so we can assume to jump directly to word after then.
			current_word = item.else_content.then_ptr + 1;

			break;
		case THEN:
			//WE AIN'T DOIN SHIT
			break;
		case WORD:
			//step through
			element = item.word_content;
			if (element->builtin == true) {
				//execute word directly
				element->_word_func(this);

			} else {
				//iterate over sub-words
				//
				
				node<crate> * item = element->crates->get_top();
				vector<crate> * next_line = new vector<crate>();
				
				//so the words appear in order, we have to flip the list arround
				linked_list<crate> * tmp = new linked_list<crate>();
				do {
					tmp->push(item->data);
					item = item->next;
				} while (item != (node<crate> *) NULL);

				//convert this to a vector
				//push current onto the return_lines stack
				//set current_line to this line
				//save the index
				//
				//after executing the sub-word, 
				//pop it back off and restore the index
				while (tmp->size() != 0) {
					next_line->push(tmp->pop());
				}
				int old_current_word = current_word;
				current_word = 0;
				return_lines.push(current_line);
				current_line = next_line;

				while (current_word < current_line->size()) {
					run_word((*current_line)[current_word]);
					current_word++;
				}
				//restore things
				current_word = old_current_word;
				current_line = return_lines.pop();

                delete(tmp);
                delete(next_line);

			}
			break;
		case INTEGER:
			//push it on the stack
			//push it REAAAALL good
			push(item.int_content);
			break;
		default:
		add_error("thread found unknown type\n");
	}
}

//TODO make new string equals function
//rather than strcmp to only check for equality

_word * interpreter::get_word(char * command) {

	node<_word *> * current = global_dict.get_top();
	while (current != NULL) {
		_word * element = current->data;
		if (strcmp(command,element->command) == 0) {
			return current->data;
		}
		current = current->next;
	}
	current = local_dict.get_top();
	while (current != NULL) {
		_word * element = current->data;
		if (strcmp(command,element->command) == 0) {
			return current->data;
		}
		current = current->next;
	}

	add_error("could not find word in dictionary: ");
	add_error(command);
	add_error("\n");
	return (_word *) NULL;

}

void interpreter::parse_line(char * input) {
	char * element = strtok(input," ");
	current_line = new vector<crate>();

	//reset the status
	bail_out = false;

	//parse the whole line into a vector
	//skip elements in ( ) 
	bool comment = false;
	while (element != NULL) {
		crate item;
		item.type = STRING;
		//TODO should probably allocate a copy of the string
		item.string_content = element;
		if (strcmp("(",element) == 0) {
			comment = true;
		}

		if (!comment) {
			current_line->push(item);
		} else if (strcmp(")",element) == 0) {
			comment = false;
		}
		element = strtok(NULL," ");
	}

	//step through each word in the list
	//with the possiblity of words making
	//us jump around
	current_word = 0;
	while (current_word < current_line->size()) {
		char * word_ptr = (*current_line)[current_word].string_content;
		if (strlen(word_ptr) <= 0 || word_ptr[0] == '\n') {
			//ignore empty string
			//and newlines

		} else if (strlen(word_ptr) >= 1 &&
			isdigit(word_ptr[0])) {
			crate number;
			number.type = INTEGER;
			number.int_content = atoi(word_ptr);
			run_word(number);
		} else {
			crate my_word;
			my_word.type = WORD;
			//find word in dict and assign it to my_word.word_content
			_word * word_tmp = get_word(word_ptr);
			if (word_tmp == (_word *) NULL) return;

			my_word.word_content = word_tmp;
			run_word(my_word);
		}
		
		current_word++;
	}

	if (bail_out) {
		add_error("function depth exceeded, bailing out");
	}

    delete(current_line);
}

