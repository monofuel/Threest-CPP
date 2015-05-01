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

	virtual ~interpreter();

	//TODO sort these
	virtual void push(int var);
	virtual void push_r(int var);
	virtual int pop();
	virtual int pop_r();
	virtual int peek();
	virtual int peek_r();
    virtual void add_word(_word);
	virtual void add_word(_word *);
    void (*add_error)(const char *);
    void (*add_output)(const char *);
	virtual void run_word(crate);
	virtual void parse_line(char *);

	virtual linked_list<_word *> get_global_dict();
    	virtual linked_list<_word *> get_local_dict();


	virtual _word * get_word(char *);

	virtual vec<crate> * get_line();
	virtual int get_current_word();
	virtual void set_current_word(int);

};

#endif
