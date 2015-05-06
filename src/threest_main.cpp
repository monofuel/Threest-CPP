#include "threest.h"
#include "threest_builtins.cpp"
#include <unistd.h>
#include <fstream>

enum run_type {INTERPRET,ONELINE,RUNFILE};

run_type our_run = INTERPRET;
bool do_load_dict = false;
string dictionary_file;
string execute_file;
bool do_persistency = false;
string persistency_file;

int error_count = 0;

//define our IO functions to pass to our interpreter
void display_output(const char * line) {
    cout << line;
}
void display_error(const char * line) {
    cerr << line;
    error_count++;
}

void output_stub(const char * line) {

}

void error_stub(const char * line) {
    error_count++;
}

void parseArguments(int argc, char * argv[]) {
    int c;
    while ((c = getopt(argc,argv,"d:p:e:lh")) != -1) {
        switch(c) {
        case 'd':
            if (strlen(optarg) == 0) {
                cout << "please give a dictionary file name" << endl;
                exit(-1);
            }
            dictionary_file = optarg;
            do_load_dict = true;

            break;
        case 'p':
            if (strlen(optarg) == 0) {
                cout << "please give a persistency file name" << endl;
                exit(-1);
            }
            do_persistency = true;
            persistency_file = optarg;
            break;
        case 'e':
            if (strlen(optarg) == 0) {
                cout << "please give a file to execute" << endl;
                exit(-1);
            }
            our_run = RUNFILE;
            execute_file = optarg;
            break;
        case 'l':
            our_run = ONELINE;
            break;
        case 'h':
            cout << "Possible arguments:" << endl;
            cout << "   -d [filename]       dictionary file to load" << endl;
            cout << "   -l                  exits after one line" << endl;
            cout << "   -p [filename]       persistency mode, error-free output is pushed to file" << endl;
            cout << "   -e [filename]       executes file and exists" << endl;
            exit(-1);
        default:
            cout << "invalid argument " << c << endl;
            exit(-1);
        }
    }

}

void execute_code(interpreter * myInter,string filename) {
    ifstream dict;
    dict.open(filename);

    //check if file exists
    if (!dict.good()) {
        if (do_load_dict) {
            cout << "invalid file: " << filename << endl;
            exit(-1);
        }
        dict.close();
        return;
    }

    //if it does, read in each line and execute it
    char * buff;
    string line;
    while (getline(dict,line)) {
        buff = (char *) malloc(sizeof(char) * line.length() + 1);
        strcpy(buff,line.c_str());
        myInter->parse_line(buff);

        if (error_count > 0) {
            cout << "error parsing " << filename << endl;
            cout << "on line: " << line << endl;
            exit(-1);
        }

        free(buff);
    }
    dict.close();
}

void interpret(interpreter * myInter) {
    char * buff;
    string line;


    while (true) {
        getline(cin,line);
        
        //this is to shut everything down and clean up
        //all memory properly. 
        //this should be done via the builtin word, however
        //that would be a pain to do cleanly.
        if (line == "BYE") {
            break;
        }
        
        buff = (char *) malloc(sizeof(char) * line.length() + 1);
        strcpy(buff,line.c_str());
        strtok(buff,"\n"); //!< destroy newline character
        myInter->parse_line(buff); //!< execute the line

        //TODO check persistency mode

        cout << " OK." << endl;
        free(buff);
    }
}

void one_liner(interpreter * myInter) {
    char * buff;
    string line;

    getline(cin,line);
    buff = (char *) malloc(sizeof(char) * line.length() + 1);
    strcpy(buff,line.c_str());
    strtok(buff,"\n"); //!< destroy newline character
    myInter->parse_line(buff); //!< execute the line

    //TODO check persistency mode

    cout << endl; //do we really want a newline?
    //cout << " OK." << endl;
    free(buff);
}

int main(int argc, char * argv[]) {

    parseArguments(argc,argv);

    interpreter myInter;
    init_builtin(&myInter);
    init_linux_builtin(&myInter);

    //supress output
    myInter.add_error = display_error;
    myInter.add_output = output_stub;
    //OPTIONAL: load dictionary file for threest-defined words
    if (do_load_dict) {
        execute_code(&myInter,dictionary_file);
        do_load_dict = false;
    }

    //load ~/.theest as the default dictionary if it exists
    execute_code(&myInter,"~/.threest");

    myInter.add_error = display_error;
    myInter.add_output = display_output;
    switch (our_run) {
    case INTERPRET:
        interpret(&myInter);
        break;
    case ONELINE:
        one_liner(&myInter);
        break;
    case RUNFILE:
        execute_code(&myInter,execute_file);
        break;
    }
}
