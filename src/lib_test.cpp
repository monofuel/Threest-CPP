#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <iostream>

//#include "hash_map.h"
#include "vector.h"
#include "linked_list.h"

using namespace std;

//----------------------------------------------------------
//test the template libraries

void hash_map_test();
void linked_list_test();
void vector_test();

int main(int argc, char * argv[]) {
	hash_map_test();
	linked_list_test();
	vector_test();
}

void hash_map_test(){
	cout << "HASH_MAP TEST TODO\n";
}

void linked_list_test(){
	cout << "LINKED_LIST TEST TODO\n";
}

void vector_test(){
	vec<int> testvec;
	int * int_list;
	int size;
	//basic tests on an empty vector
	
	assert(testvec.size() == 0);
	testvec.clear();
	assert(testvec.size() == 0);
	int_list = testvec.list();
	assert(int_list == (int*) NULL);
	
	//init checks
	int fixed_list[5] = {1,2,3,4,5};
	size = 5;
	vec<int> fixedvec(fixed_list,size);
	assert(fixedvec.size() == size);
	for (int i = 0; i < size; i++) {
		assert(fixedvec[i] == fixed_list[i]);
	}
	
	//push and pop checks
	int add_list[10];
	//create a random list
	for (int i = 0; i < 10; i++) {
		add_list[i] = rand();
	}
	//pop a value off
	assert(5 == fixedvec.pop());
	//push random data
	for (int i = 0; i < 10; i++) {
		fixedvec.push(add_list[i]);
	}
	//assert 4 old values
	for (int i = 0; i < 4; i++) {
		assert(fixedvec[i] == fixed_list[i]);
	}
	//assert added values
	for (int i = 0; i < 10; i++) {
		assert(fixedvec[i+4] == add_list[i]);
	}
	//pop all values off
	while (fixedvec.size() > 0) {
		fixedvec.pop();
	}
	assert(fixedvec.size() == 0);
	
	//add random data back
	for (int i = 0; i < 10; i++) {
		fixedvec.push(add_list[i]);
	}
	//assert added values
	for (int i = 0; i < 10; i++) {
		assert(fixedvec[i] == add_list[i]);
	}
	
	//copy to a second vec
	vec<int> vec_copy = fixedvec;
	//pop values off
	for (int i = 0; i < 5; i++) {
		fixedvec.pop();
	}
	//assert values from copy
	for (int i = 0; i < 10; i++) {
		assert(vec_copy[i] == add_list[i]);
	}
	
	cout << "vector test success!\n";
}