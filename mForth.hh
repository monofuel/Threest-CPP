#ifndef MFORTH_H
#define MFORTH_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

//words can either be builtin
//or made up of other words.
typedef struct {
	string command;
	bool builtin;
	union {
	void (*word_func)(void);
	vector<string>* words;
	};

} word;

vector<int> stack;
vector<word> dictionary;
bool building_word = false;
vector<string> building_words;

void push(int);
int pop();
int peek();

void display_1();

void add_word(word);
void init_builtin();

int main(int, char **);

#endif
