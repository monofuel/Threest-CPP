#ifndef MFORTH_H
#define MFORTH_H

#ifndef  __AVR_ARCH__ 
#include <iostream>
#include <vector>
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace std;

//words can either be builtin
//or made up of other words.
typedef struct {
	const char * command;
	bool builtin;
	union {
	void (*word_func)(void);
	vector<const char *>* words;
	};

} word;

vector<word> get_dictionary();
void push(int);
int pop();
int peek();

void display_1();

void add_word(word);
void run_word(const char *);
void init_builtin();

int main(int, char **);

#endif
