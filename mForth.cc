#include "mForth.hh"
#include "mForth_builtins.cc"


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
void run_word(string command) {
	for (word element : dictionary) {
		if (command.compare(element.command) == 0) {
			if (element.builtin) {
				//execute function
				element.word_func();
			} else {
				//execute sub-words
				for (string sub_word : *element.words) run_word(sub_word);
			}
			return;
		}
	}
	//if word isn't found
	cout << "Word not found: " << command << " ";

}

//building_word is a bool to help with
//multi-line word definitions

void parse_line(string input) {
	char * input_array = const_cast<char*>(input.c_str());
	char * element = strtok(input_array," ");
	do {
		if (strcmp(element,":") == 0) {
			building_word = true;
			continue;
		}
		if (building_word) {
			if (strcmp(element,";") == 0) {
				break;
			}
			building_words.push_back(element);
		} else {
			if (input.length() >= 1 && isdigit(element[0])) {
				push(stoi(element, (size_t *)NULL,10));
			} else {
				run_word(element);
			}
		}
	} while ((element = strtok(NULL," ")) != NULL);
}

int main(int argc, char * argv[]) {

	init_builtin();

	while (true) {
		string input;

		getline(cin,input);
		parse_line(input);
		cout << " OK." << endl;
	}
}
