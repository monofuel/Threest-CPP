#include "threest.h"

int main(int argc, char * argv[]) {

	char * input;
	size_t size;
	const char * output;

	//TODO we should handle command line arguments

	interpreter myInter; //!< our interpreter instance
	init_builtin(myInter); //!< populate default words


	while (true) {
		input =(char *)  NULL;
		size = 0;
		getline(&input,&size,stdin);
		strtok(input,"\n"); //!< destroy newline character
		myInter.parse_line(input); //!< execute the line
		output = myInter.get_error();
		if (output != (const char *) NULL) {
			do {
				cout << output;
			} while(output = myInter.get_error());

		} else if ((output = myInter.get_output()) != (const char *) NULL) {
			do {
				cout << output;
			} while(output = myInter.get_output());
		}

		cout << " OK." << endl;
		free(input);
	}
}
