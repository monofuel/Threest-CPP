#include "threest.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class interpreter {

protected:

	//this should be static
	linked_list<_word *> global_dict;

    //TODO replace stack from ints to crates
	linked_list<int> stack;
	linked_list<int> return_stack;
	linked_list<_word *> local_dict;
	vector<crate> * current_line;
	linked_list<vector<crate>*> return_lines;
	int current_word;
	linked_list<const char *> errors_queue;
	linked_list<const char *> output_queue;


public:

	//TODO sort these
	virtual void push(int var);
	virtual int pop();
	virtual int peek();
	virtual void add_word(_word);
	virtual void add_word(_word *);
	virtual const char * get_error();
	virtual const char * get_output();
	virtual void add_error(const char *);
	virtual void add_output(const char *);
	virtual void run_word(crate);
	virtual void parse_line(char *);

	virtual linked_list<_word *> get_global_dict();
    	virtual linked_list<_word *> get_local_dict();

	virtual int get_error_count();
	virtual int get_output_count();

	virtual _word * get_word(char *);

	virtual vector<crate> * get_line();
	virtual int get_current_word();
	virtual void set_current_word(int);

};

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

const char * interpreter::get_error() {
	return errors_queue.pop();
}
const char * interpreter::get_output() {
	return output_queue.pop();
}

int interpreter::get_error_count() {
	return errors_queue.size();
}

int interpreter::get_output_count() {
	return output_queue.size();
}

void interpreter::add_error(const char * line) {
	errors_queue.append(line);
}

void interpreter::add_output(const char * line) {
	output_queue.append(line);
}

void interpreter::push(int var) {
	stack.push(var);
}

int interpreter::pop() {
	int value;
	if (stack.size() == 0) {
		//error case
		errors_queue.append("stack underflow\n");
		return 0;
	} else {
		value = stack.get_top()->data;
		stack.pop();
	}
	return value;
}

int interpreter::peek() {
	return stack.get_top()->data;
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
					errors_queue.append("cannot redefine built-in words\n");
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
					errors_queue.append("cannot redefine built-in words");
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
	_word * element;
	//execute the item
	switch (item.type) {
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

			}
			break;
		case INTEGER:
			//push it on the stack
			//push it REAAAALL good
			push(item.int_content);
			break;
		default:
		5 + 5;
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

	add_error("could not find word in dictionary\n");
	add_error(command);
	return (_word *) NULL;

}

void interpreter::parse_line(char * input) {
	char * element = strtok(input," ");
	current_line = new vector<crate>();

	//parse the whole line into a vector
	while (element != NULL) {
		crate item;
		item.type = STRING;
		//TODO should probably allocate a copy of the string
		item.string_content = element;
		current_line->push(item);
		element = strtok(NULL," ");
	}

	//step through each word in the list
	//with the possiblity of words making
	//us jump around
	current_word = 0;
	while (current_word < current_line->size()) {
		char * word_ptr = (*current_line)[current_word].string_content;
		if (strlen(word_ptr) == 0) continue;
		if (strlen(word_ptr) >= 1 &&
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

}


#endif
