#ifndef MFORTH_H
#define MFORTH_H

#ifndef  __AVR_ARCH__ 
#include <iostream>
#endif

#ifdef __AVR_ARCH__
//suck it
#ifndef NULL
#define NULL null
#endif
#include <Arduino.h>
#endif

#include "linked_list.h"

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
