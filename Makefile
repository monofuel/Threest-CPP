Threest: src/threest_main.cpp src/threest.h src/threest_builtins.cpp src/linked_list.h
	g++ -g -pedantic -std=c++11 -o bin/threest src/threest_main.cpp
clean:
	rm bin/threest || true
