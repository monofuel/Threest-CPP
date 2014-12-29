#include "mForth.h"

void display_1() {
	#ifdef  __AVR_ARCH__
	Serial.print(pop());
	Serial.print(" ");
	#else
	cout << pop() << " ";
	#endif
}

void bye() {
	exit(0);
}

void cr() {
	#ifdef __AVR_ARCH__
	Serial.println();
	#else
	cout << endl;
	#endif
}

void words() {
	node<_word> * element = get_dictionary().get_top();
	while (element != NULL) {
		#ifdef __AVR_ARCH__
		Serial.print(element->data.command);
		Serial.print(" ");
		#else
		cout << element->data.command << " ";
		#endif
		element = element->next;
	}
}

void drop() {
	pop();
}

void dup() {
	int var = pop();
	push(var);
	push(var);

}

void is_equal() {
	int var1 = pop();
	int var2 = pop();
	push(var2 == var1);
}

void greater_than() {
	int var1 = pop();
	int var2 = pop();
	push(var2 > var1);
}

void less_than() {
	int var1 = pop();
	int var2 = pop();
	push(var2 < var1);
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
	_word tmp;

	tmp.command = "DUP";
	tmp.builtin = true;
	tmp._word_func = &dup;
	add_word(tmp);

	tmp.command = "<";
	tmp.builtin = true;
	tmp._word_func = &less_than;
	add_word(tmp);

	tmp.command = ">";
	tmp.builtin = true;
	tmp._word_func = &greater_than;
	add_word(tmp);

	tmp.command = "==";
	tmp.builtin = true;
	tmp._word_func = &is_equal;
	add_word(tmp);

	tmp.command = "/";
	tmp.builtin = true;
	tmp._word_func = &divide;
	add_word(tmp);

	tmp.command = "*";
	tmp.builtin = true;
	tmp._word_func = &multiply;
	add_word(tmp);

	tmp.command = "-";
	tmp.builtin = true;
	tmp._word_func = &sub;
	add_word(tmp);

	tmp.command = "+";
	tmp.builtin = true;
	tmp._word_func = &add;
	add_word(tmp);

	tmp.command = ".";
	tmp.builtin = true;
	tmp._word_func = &display_1;
	add_word(tmp);

	tmp.command = "DROP";
	tmp.builtin = true;
	tmp._word_func = &drop;
	add_word(tmp);

	tmp.command = "CR";
	tmp.builtin = true;
	tmp._word_func = &cr;
	add_word(tmp);

	tmp.command = "WORDS";
	tmp.builtin = true;
	tmp._word_func = &words;
	add_word(tmp);

	tmp.command = "BYE";
	tmp.builtin = true;
	tmp._word_func = &bye;
	add_word(tmp);
}
