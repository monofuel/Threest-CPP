#include "threest.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class interpreter {

protected:

	//this should be static
	linked_list<_word> global_dict;

    //TODO replace stack from ints to crates
	linked_list<int> stack;
	linked_list<_word> local_dict;
	vector<char *> current_line;
	int current_word;
	linked_list<const char *> errors_queue;
	linked_list<const char *> output_queue;


public:

	//TODO sort these
	virtual void push(int var);
	virtual int pop();
	virtual int peek();
	virtual void add_word(_word);
	virtual const char * get_error();
	virtual const char * get_output();
	virtual void add_error(const char *);
	virtual void add_output(const char *);
	virtual void run_word(crate);
	virtual void parse_line(char *);

	virtual linked_list<_word> get_global_dict();
    	virtual linked_list<_word> get_local_dict();

	virtual int get_error_count();
	virtual int get_output_count();


};

linked_list<_word> interpreter::get_global_dict() {
    return global_dict;
}

linked_list<_word> interpreter::get_local_dict() {
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
		errors_queue.append("stack underflow");
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

	//check if it will be global or not
	if (item.command[0] == '#') {
		//local
		node<_word> * current = local_dict.get_top();
		while (current != NULL) {
			if (strcmp(current->data.command,item.command) == 0) {
				if (current->data.builtin) {
					errors_queue.append("cannot redefine built-in words");
					return;
				}
				free(current->data.crates);
				current->data = item;
			}
			current = current->next;
		}
		local_dict.push(item);
	} else {
		//global
		node<_word> * current = global_dict.get_top();
		while (current != NULL) {
			if (strcmp(current->data.command,item.command) == 0) {
				if (current->data.builtin) {
					errors_queue.append("cannot redefine built-in words");
					return;
				}
				free(current->data.crates);
				current->data = item;
			}
			current = current->next;
		}
		global_dict.push(item);
	}
}

void interpreter::run_word(crate item) {
	//execute the item
	switch (item.type) {
		case WORD:

		case INTEGER:


		default:

	}
}

//building__word is a bool to help with
//multi-line _word definitions

void interpreter::parse_line(char * input) {
	char * element = strtok(input," ");
	current_line.clear();

	//parse the whole line into a vector
	while (element != NULL) {
		current_line.push(element);
		element = strtok(NULL," ");
	}

	//step through each word in the list
	//with the possiblity of words making
	//us jump around
	current_word = 0;
	while (current_word < current_line.size()) {
		//TODO
		//package up a new crate for each
		//item being defined to run
		//run_word(current_line[current_word]);
		current_word++;
	}
}


#endif
