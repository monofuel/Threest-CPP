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
	cout << "HASH_MAP TEST TODO" << endl;
}

void linked_list_test(){

	linked_list<int> testlist;
	//basic tests on an empty list
	
	assert(testlist.size() == 0);
	testlist.clear();
	assert(testlist.size() == 0);
	
	//init checks
	int fixed_list[5] = {1,2,3,4,5};
	int size = 5;
	for (int i = 0; i < size; i++) {
		testlist.push(fixed_list[i]);
	}
	int * list_copy = testlist.list();
	for (int i = 0; i < size; i++) {
		assert(fixed_list[i] == list_copy[4-i]);
	}
	free(list_copy);
	
	//push and pop checks
	int add_list[10];
	//create a random list
	for (int i = 0; i < 10; i++) {
		add_list[i] = rand();
	}
	//pop a value off
	assert(5 == testlist.pop());
	//push random data
	for (int i = 0; i < 10; i++) {
		testlist.push(add_list[i]);
	}
	//assert 4 old values
	for (int i = 10; i < 14; i++) {
		//cout << i << ":" << testlist[i] << "," << fixed_list[13-i] << endl;
		assert(testlist[i] == fixed_list[13 - i]);
	}
	//assert added values
	for (int i = 0; i < 10; i++) {
		assert(testlist[i] == add_list[9-i]);
	}
	
	//copy to a second vec
	linked_list<int> testlist_copy = testlist;
	//pop values off
	
	for (int i = 0; i < 5; i++) {
		testlist.pop();
	}
	assert(testlist.size() == 9);
	assert(testlist_copy.size() == 14);
	//assert values from copy
	for (int i = 0; i < 9; i++) {
		//cout << i << ":" << testlist_copy[i+5] << "," << testlist[i] << endl;
		assert(testlist_copy[i+5] == testlist[i]);
	}
	
	cout << "linked list test success!" << endl;
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
	assert(fixedvec.size() == 5);
	assert(vec_copy.size() == 10);
	//assert values from copy
	for (int i = 0; i < 10; i++) {
		assert(vec_copy[i] == add_list[i]);
	}
	
	cout << "vector test success!" << endl;
}