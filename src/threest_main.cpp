#include "threest.h"
#include "threest_builtins.cpp"

int main(int argc, char * argv[]) {

	char * input;
	size_t size;
	const char * output;

	//TODO we should handle command line arguments

	interpreter myInter; //!< our interpreter instance
	init_builtin(&myInter); //!< populate default words


	while (true) {
		input =(char *)  NULL;
		size = 0;
		getline(&input,&size,stdin);
		strtok(input,"\n"); //!< destroy newline character
		myInter.parse_line(input); //!< execute the line
		while (myInter.get_error_count() > 0) {
			cout << myInter.get_error();
		}
		while (myInter.get_output_count() > 0) {
			cout << myInter.get_output();
		}

		cout << " OK." << endl;
		free(input);
	}
}
