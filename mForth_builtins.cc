#include "mForth.hh"

void display_1() {
	cout << pop() << " ";
}

void bye() {
	exit(0);
}

void cr() {
	cout << endl;
}

void words() {
	for (word element : dictionary) {
		cout << element.command << " ";
	}
}

void add() {
	int var1 = pop();
	int var2 = pop();
	push(var2 + var1);
}

void sub() {
	int var1 = pop();
	int var2 = pop();
	push(var2 - var1);
}

void multiply() {
	int var1 = pop();
	int var2 = pop();
	push(var2 * var1);
}

void divide() {
	int var1 = pop();
	int var2 = pop();
	push(var2 / var1);
}

void init_builtin() {
	word tmp;

	tmp.command = "/";
	tmp.builtin = true;
	tmp.word_func = &divide;
	add_word(tmp);

	tmp.command = "*";
	tmp.builtin = true;
	tmp.word_func = &multiply;
	add_word(tmp);

	tmp.command = "-";
	tmp.builtin = true;
	tmp.word_func = &sub;
	add_word(tmp);

	tmp.command = "+";
	tmp.builtin = true;
	tmp.word_func = &add;
	add_word(tmp);

	tmp.command = ".";
	tmp.builtin = true;
	tmp.word_func = &display_1;
	add_word(tmp);

	tmp.command = "CR";
	tmp.builtin = true;
	tmp.word_func = &cr;
	add_word(tmp);

	tmp.command = "words";
	tmp.builtin = true;
	tmp.word_func = &words;
	add_word(tmp);

	tmp.command = "bye";
	tmp.builtin = true;
	tmp.word_func = &bye;
	add_word(tmp);	
}
