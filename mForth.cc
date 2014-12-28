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

	if (!is_comment && command.compare("(") == 0) {
		is_comment = true;
		return;
	}else if (is_comment && command.compare(")") == 0) {
		is_comment = false;
		return;
	}else if (is_comment) return;
	
	if (command.length() >= 1 && isdigit(command[0])) {
		if (if_stack.size() == 0 || if_stack.back())
			push(stoi(command, (size_t *)NULL,10));
		return;
	} else {
		if (command.compare("IF") == 0) {
			if_stack.push_back(pop());
			return;
		} else if (command.compare("THEN") == 0) {
			if_stack.pop_back();	
			return;
		} else if (command.compare("ELSE") == 0) {
			//invert latest if statement
			//if true else false then
			//if false else true then
			if_stack.back() = !if_stack.back();
			return;
		}
		for (word element : dictionary) {
			if (command.compare(element.command) == 0) {
				if (element.builtin) {
					//execute function
					if (if_stack.size() == 0) {
						element.word_func();
					} else if (if_stack.back()) {
						element.word_func();
					}
				} else {
					//execute sub-words
					for (string sub_word : *element.words) {
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

void parse_line(string input) {
	char * input_array = const_cast<char*>(input.c_str());
	char * element = strtok(input_array," ");
	do {
		if (strcmp(element,":") == 0) {
			building_word = true;
			tmp_word.words = (vector<string> *) calloc(1,sizeof(vector<string>));

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

int main(int argc, char * argv[]) {

	init_builtin();

	while (true) {
		string input;

		getline(cin,input);
		parse_line(input);
		cout << " OK." << endl;
	}
}
