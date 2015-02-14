#include "threest.h"
#include "threest_builtins.cpp"
#include <unistd.h>

enum run_type {INTERPRET,ONELINE,RUNFILE};

run_type our_run = INTERPRET;
bool do_load_dict = false;
string dictionary;

string execute_file;

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
    int c;
    while ((c = getopt(argc,argv,"d:p:e:l"))) {
        switch(c) {
        case 'd':
            if (strlen(optarg) == 0) {
                cout << "please give a dictionary file name" << endl;
                exit(0);
            }
            dictionary = optarg;
            do_load_dict = true;

            break;
        case 'p':
            if (strlen(optarg) == 0) {
                cout << "please give a persistency file name" << endl;
                exit(0);
            }
            cout << "TODO" << endl;
            break;
        case 'e':
            if (strlen(optarg) == 0) {
                cout << "please give a file to execute" << endl;
                exit(0);
            }
            break;
        case 'l':
            our_run = ONELINE;
            break;
        default:
            cout << "invalid argument " << c << endl;
        case 'h':
            cout << "Possible arguments:" << endl;
            cout << "   -d [filename]       dictionary file to load" << endl;
            cout << "   -l                  exits after one line" << endl;
            cout << "   -p [filename]       persistency mode, error-free output is added to file" << endl;
            cout << "   -e [filename]       executes file and exists" << endl;
            exit(0);
        }
    }

}

void interpret(interpreter * myInter) {

    char * input;
    size_t size;
    const char * output;

    myInter->add_error = display_error;
    myInter->add_output = display_output;

    while (true) {
        input =(char *)  NULL;
        size = 0;
        getline(&input,&size,stdin);
        strtok(input,"\n"); //!< destroy newline character
        myInter->parse_line(input); //!< execute the line

        cout << " OK." << endl;
        free(input);
    }
}

int main(int argc, char * argv[]) {

    parseArguments(argc,argv);

    interpreter myInter;
    init_builtin(&myInter);

    //OPTIONAL: load dictionary file for threest-defined words
    if (do_load_dict) {

    }

    //TODO: load ~/.theest as the default dictionary if it exists

    switch (our_run) {
    case INTERPRET:
        interpret(&myInter);
        break;
    case ONELINE:
        break;
    case RUNFILE:
        break;
    }
}
