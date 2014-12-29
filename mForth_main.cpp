#include "mForth.h"

linked_list<int> stack;
linked_list<bool> if_stack;
linked_list<word> dictionary;
bool is_comment = false;
bool building_word = false;
word tmp_word;

linked_list<word> get_dictionary() {
	return dictionary;
}

void push(int var) {
	stack.push(var);
}

int pop() {
	int value;
	if (stack.size() == 0) {
		//error case
		cout << "stack underflow" << endl;
		return 0;
	} else {
		value = stack.get_top()->data;
		stack.pop();
	}
	return value;
}

int peek() {
	return stack.get_top()->data;
}

void add_word(word item) {
	dictionary.push(item);
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
		if (if_stack.size() == 0 || if_stack.get_top()->data)
			push(atoi(command));
		return;
	} else {
		if (strcmp(command,"IF") == 0) {
			if_stack.push(pop());
			return;
		} else if (strcmp(command,"THEN") == 0) {
			if_stack.pop();	
			return;
		} else if (strcmp(command,"ELSE") == 0) {
			//invert latest if statement
			//if true else false then
			//if false else true then
			if_stack.get_top()->data = !if_stack.get_top()->data;
			return;
		}
		node<word> * current = get_dictionary().get_top();
		while (current != NULL) {
			word element = current->data;
			if (strcmp(command,element.command) == 0) {
				if (element.builtin) {
					//execute function
					if (if_stack.size() == 0) {
						element.word_func();
					} else if (if_stack.get_top()) {
						element.word_func();
					}
				} else {
					//execute sub-words
					node <const char *> * current_word = element.words->get_top();
					while (current_word != NULL) {
						run_word(current_word->data);
						current_word = current_word->next;
					}
				}
				return;
			}
			current = current->next;
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
			tmp_word.words = new linked_list<const char *>();
			char * tmp_command = strtok(NULL," ");
			char * buffer = (char *) calloc(strlen(tmp_command)+1,sizeof(char));
			strcpy(buffer,tmp_command);
			tmp_word.command = buffer;
			element = strtok(NULL," ");
		}
		if (building_word) {
			if (strcmp(element,";") == 0) {
				building_word = false;
				add_word(tmp_word);
				break;
			}
			char * buffer = (char *) calloc(strlen(element)+1,sizeof(char));
			strcpy(buffer,element);
			tmp_word.words->append(buffer);
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
		free(input);
	}
}
#endif
