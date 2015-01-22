#include "mForth.h"

linked_list<int> stack;
linked_list<bool> if_stack;
linked_list<_word> dictionary;
bool is_comment = false;
bool building__word = false;
_word tmp__word;

linked_list<_word> get_dictionary() {
	return dictionary;
}

void push(int var) {
	stack.push(var);
}

int pop() {
	int value;
	if (stack.size() == 0) {
		//error case
		#ifdef  __AVR_ARCH__
		Serial.println("stack underflow");
		#else
		cout << "stack underflow" << endl;
		#endif
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

void add_word(_word item) {
	//check if exists
	node<_word> * current = get_dictionary().get_top();
	while (current != NULL) {
		if (strcmp(current->data.command,item.command) == 0) {
			if (current->data.builtin) {
				#ifdef __AVR_ARCH__
				Serial.println("cannot redefine built-in words");
				#else
				cout << "cannot redefine built-in words" << endl;
				#endif
				return;
			}
			free(current->data.words);
			current->data = item;
		}
		current = current->next;
	}
	dictionary.push(item);
}
void run__word(const char * command) {
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
		node<_word> * current = get_dictionary().get_top();
		while (current != NULL) {
			_word element = current->data;
			if (strcmp(command,element.command) == 0) {
				if (element.builtin) {
					//execute function
					if (if_stack.size() == 0) {
						element._word_func();
					} else if (if_stack.get_top()) {
						element._word_func();
					}
				} else {
					//execute sub-words
					node <const char *> * current__word = element.words->get_top();
					while (current__word != NULL) {
						run__word(current__word->data);
						current__word = current__word->next;
					}
				}
				return;
			}
			current = current->next;
		}
	}
	//if _word isn't found
	#ifdef  __AVR_ARCH__
	Serial.print("_word not found: ");
	Serial.println(command);
	#else
	cout << "_word not found: " << command << " ";
	#endif

}

//building__word is a bool to help with
//multi-line _word definitions

void parse_line(char * input) {
	char * element = strtok(input," ");
	if (element == NULL) return;
	do {
		if (strcmp(element,":") == 0) {
			building__word = true;
			tmp__word.words = new linked_list<const char *>();
			char * tmp_command = strtok(NULL," ");
			char * buffer = (char *) calloc(strlen(tmp_command)+1,sizeof(char));
			strcpy(buffer,tmp_command);
			tmp__word.command = buffer;
			element = strtok(NULL," ");
		}
		if (building__word) {
			if (strcmp(element,";") == 0) {
				building__word = false;
				add_word(tmp__word);
				break;
			}
			char * buffer = (char *) calloc(strlen(element)+1,sizeof(char));
			strcpy(buffer,element);
			tmp__word.words->append(buffer);
		} else {
			run__word(element);
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
