mForth: mForth_main.cpp mForth.h mForth_builtins.cpp linked_list.h
	g++ -pedantic -std=c++11 -o mForth mForth_main.cpp mForth_builtins.cpp
