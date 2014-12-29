mForth: mForth_main.cpp mForth.h mForth_builtins.cpp
	g++ -pedantic -std=c++11 -o mForth mForth_main.cpp mForth_builtins.cpp
