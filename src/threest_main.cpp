#include "threest.h"
#include "threest_builtins.cpp"
#include <unistd.h>


int error_count = 0;

//define our IO functions to pass to our interpreter
void display_output(const char * line) {
    cout << line;
}
void display_error(const char * line) {
    cerr << line;
    error_count++;
}

void parseArguments(int argc, char * argv[]) {

}

int main(int argc, char * argv[]) {

	char * input;
	size_t size;
	const char * output;

    parseArguments(argc,argv);

	interpreter myInter; //!< our interpreter instance
	init_builtin(&myInter); //!< populate default words

    myInter.add_error = display_error;
    myInter.add_output = display_output;

	while (true) {
		input =(char *)  NULL;
		size = 0;
		getline(&input,&size,stdin);
		strtok(input,"\n"); //!< destroy newline character
		myInter.parse_line(input); //!< execute the line

		cout << " OK." << endl;
		free(input);
	}
}
