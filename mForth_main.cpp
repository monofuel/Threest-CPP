#include "mForth.h"

vector<int> stack;
vector<bool> if_stack;
vector<word> dictionary;
bool is_comment = false;
bool building_word = false;
word tmp_word;

vector<word> get_dictionary() {
	return dictionary;
}

void push(int var) {
	stack.push_back(var);
}

int pop() {
	int value;
	if (stack.size() == 0) {
		//error case
		cout << "stack underflow" << endl;
		return 0;
	} else {
		value = stack.back();
		stack.pop_back();
	}
	return value;
}

int peek() {
	return stack.back();
}
void add_word(word item) {
	dictionary.push_back(item);
}
void run_word(const char * command) {

	if (!is_comment && strcmp(command,"(") == 0) {
		is_comment = true;
		return;
	}else if (is_comment && strcmp(command,")") == 0) {
		is_comment = false;
		return;
	}else if (is_comment) return;
	
	if (strlen(command) >= 1 && isdigit(command[0])) {
		if (if_stack.size() == 0 || if_stack.back())
			push(atoi(command));
		return;
	} else {
		if (strcmp(command,"IF") == 0) {
			if_stack.push_back(pop());
			return;
		} else if (strcmp(command,"THEN") == 0) {
			if_stack.pop_back();	
			return;
		} else if (strcmp(command,"ELSE") == 0) {
			//invert latest if statement
			//if true else false then
			//if false else true then
			if_stack.back() = !if_stack.back();
			return;
		}
		for (word element : dictionary) {
			if (strcmp(command,element.command) == 0) {
				if (element.builtin) {
					//execute function
					if (if_stack.size() == 0) {
						element.word_func();
					} else if (if_stack.back()) {
						element.word_func();
					}
				} else {
					//execute sub-words
					for (const char * sub_word : *element.words) {
						run_word(sub_word);
					}
				}
				return;
			}
		}
	}
	//if word isn't found
	cout << "Word not found: " << command << " ";

}

//building_word is a bool to help with
//multi-line word definitions

void parse_line(char * input) {
	char * element = strtok(input," ");
	if (element == NULL) return;
	do {
		if (strcmp(element,":") == 0) {
			building_word = true;
			tmp_word.words = (vector<const char *> *) calloc(1,sizeof(vector<char *>));

			tmp_word.command = strtok(NULL," ");
			element = strtok(NULL," ");
			
		}
		if (building_word) {
			if (strcmp(element,";") == 0) {
				building_word = false;
				add_word(tmp_word);
				break;
			}
			tmp_word.words->push_back(element);
		} else {
			run_word(element);
		}
	} while ((element = strtok(NULL," ")) != NULL);
}
#ifndef __AVR_ARCH__
int main(int argc, char * argv[]) {

	init_builtin();

	while (true) {
		char * input =(char *)  NULL;
		size_t size = 0;
		getline(&input,&size,stdin);
		//destroy newline
		strtok(input,"\n");
		parse_line(input);
		cout << " OK." << endl;
	}
}
#endif
