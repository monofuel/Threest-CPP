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
	vector<crate> * current_line;
	linked_list<vector<crate>*> return_lines;
	int current_word;
	linked_list<const char *> errors_queue;
	linked_list<const char *> output_queue;

	bool bail_out = false;

public:

	//TODO sort these
	virtual void push(int var);
	virtual void push_r(int var);
	virtual int pop();
	virtual int pop_r();
	virtual int peek();
	virtual int peek_r();
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

#endif
