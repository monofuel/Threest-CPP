#include "threest.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class interpreter {

protected:


	static linked_list<_word *> global_dict;

	//TODO replace stack from ints to crates
	linked_list<int> stack;
	linked_list<int> return_stack;
	linked_list<_word *> local_dict;
	vec<crate> * current_line;
	linked_list<vec<crate>*> return_lines;
	int current_word;

	bool bail_out = false;

public:

	~interpreter();

	//TODO sort these
	void push(int var);
	void push_r(int var);
	int pop();
	int pop_r();
	int peek();
	int peek_r();
    void add_word(_word);
	void add_word(_word *);
    void (*add_error)(const char *);
    void (*add_output)(const char *);
	void run_word(crate);
	void parse_line(char *);

	linked_list<_word *> get_global_dict();
    linked_list<_word *> get_local_dict();


	_word * get_word(char *);

	vec<crate> * get_line();
	int get_current_word();
	void set_current_word(int);

};

#endif
