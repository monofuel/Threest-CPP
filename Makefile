#AB 2-14-2015
#THREEST

CC=g++
CFLAGS=-pedantic -std=c++11
CFLAGS+=-g #feel free to comment this out
#CFLAGS+=-O2 #going for distance
#CFLAGS+=-march=native #going for SPEEEEEED
OUTPUT=bin/threest
SRC=src/threest_main.cpp src/interpreter.cpp src/threest_linux_builtins.cpp


all: bin/threest

test: bin/threest

	#"------------------------------------"
	#"		Performing Tests"
	#"------------------------------------"
	
	#verify it starts
	echo "BYE" | ./bin/threest
	#test dictionary file
	echo "BYE" | ./bin/threest -d tests/dictionary.txt
	#test ~/.threest dictionary
	touch ~/.threest
	echo "BYE" | ./bin/threest -d ~/.threest
	#test oneline mode
	#if this hangs, this means it failed.
	echo "" | ./bin/threest -l
	#test execution mode
	./bin/threest -e tests/empty.txt

	#TODO
	#test ints and display
	echo "1 2 3 . . ." | ./bin/threest -l
	#test algebra
	echo "1 2 3 + + ." | ./bin/threest -l
	#test defining functions
	echo ": PEEK2 DUP DUP . . ; 5 PEEK2" | ./bin/threest -l
	#test words before function definitions
	echo " 5 : PEEK2 DUP DUP . . ; PEEK2" | ./bin/threest -l
	#test branches
	echo ": TEST 0 IF 1 . ELSE 0 . THEN ; TEST" | ./bin/threest -l
	#test branches
	echo ": TEST 1 IF 1 . ELSE 0 . THEN ; TEST" | ./bin/threest -l
	#test loops
	echo ": TEST 10 0 DO I . LOOP ; TEST" | ./bin/threest -l
	#test recursion
	echo ": TEST 1 - DUP = IF ELSE RECURSE THEN ; 5 TEST" | ./bin/threest -l

	#test fibonacci number generator
	echo "5 FIB ." | ./bin/threest -d tests/dictionary.txt -l

	#test persistency mode
	
	#FOR FUTURE FEATURES
	#test floats
	#test strings
	#test arrays
	#test json

	#test cross-type math


	#"------------------------------------"
	#"		Test SUCCESS"
	#"------------------------------------"


bin/threest: src/threest_main.cpp src/threest.h src/threest_builtins.cpp src/linked_list.h
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)
clean:
	rm $(OUTPUT) || true
