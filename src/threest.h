#ifndef THREEST_H
#define THREEST_H

#ifndef  __AVR_ARCH__ 
#include <iostream>
#include <cstddef>
#include <cstdlib>
#endif

#ifdef __AVR_ARCH__
//suck it
#ifndef NULL
#define NULL null
#endif
#include <Arduino.h>
#endif

#include "linked_list.h"
#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace std;

//_words can either be builtin
//or made up of other _words.
typedef struct {
	const char * command;
	bool builtin;
	union {
	void (*_word_func)(void);
	linked_list<const char *>* words;
	};

} _word;
//arduino has a type named word, so we have to use _word

//the if crate contains a pointer to the else word.
//if the stack is true, continue until you hit else, then skip to then.
//otherwise, jump to the else pointer.
typedef struct {
	int else_ptr;
} _if;

typedef struct {
	int then_ptr;
} _else;

//do doesn't need to store anything important
//typedef struct {
//} _do;

//loop contains a pointer to do
typedef struct {
	int do_ptr;
} _loop;

//crates are the basic building block of compiled threest code.
//when a crate is executed, it's type is checked to determine 
//what to do. words and flow control are interpreted, while
//ints, arrays, objects and other non-instructions are pushed
//onto the stack.
enum crate_type { WORD,IF,ELSE,DO,LOOP,INTEGER,FLOAT,ARRAY,OBJECT,STRING,BOOL } ;
typedef struct {
	crate_type type;
	union {
		_word word_content;
		_if if_content;
		_else else_content;
		_loop loop_content;
		int int_content;
		float float_content;
		char * string_content;
		bool bool_content;
	};
} crate;

linked_list<_word> get_dictionary();
void push(int);
int pop();
int peek();

void display_1();

void add_word(_word);
void run_word(const char *);
void init_builtin();
void init_ardu_builtin();

void parse_line(char *);

int main(int, char **);

#endif
