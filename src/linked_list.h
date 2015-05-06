//can't use these on arduino
//#include <cstddef>
//#include <cstdlib>

#ifdef __AVR_ARCH__

#include <assert.h>
//suck it
#ifndef NULL
#define NULL null
#endif

#else //if not arduino
#include <cassert>
#endif

#ifndef  LINKEDLIST_H
#define  LINKEDLIST_H

//linked list data structure
//data,pointer to next
template<class T>
struct node {
	T data;
	node<T> * next;
};

template<typename T>
class linked_list {

protected:

	node<T> * top; //pointer to first element of the list
	int _size;
	
	int last; //index of last element
	//last will be -1 if it needs to be reset
	node<T> * last_node; //iterator pointer
	

public:
	linked_list();
	linked_list(const linked_list<T>&);
	~linked_list();

	int size();
	
	void push(T);
	T pop();
	
	void clear();
	T* list();
	
	//fetch items from inside the list
	//uses last and last_node for faster iterating
	T operator[](int);

};

//construct an empty linked list
template<typename T>
linked_list<T>::linked_list() {
	top = (node<T> *) NULL;
	_size = 0;
	last = -1;
}

//create linked_list from another
template<typename T>
linked_list<T>::linked_list(const linked_list<T> &other) {
	top = (node<T> *) NULL;
	_size = 0;
	last = -1;
	
	node<T> * other_current = other.top;
	node<T> * current = new node<T>;
	top = current;
	top->data = other.top->data;
	_size++;
	for (int i = 0; i < other._size-1; i++) {
		//advance the list
		other_current = other_current->next;
		//make sure our current nodes are valid
		assert(current != (node<T>*) NULL);
		assert(other_current != (node<T>*) NULL);
		
		//allocate a new node
		current->next = new node<T>;
		_size++;
		current = current->next;
		
		//copy the data over
		current->data = other_current->data;
	}
}

//destructor
template<typename T>
linked_list<T>::~linked_list() {
	this->clear();
}

//get the total size of the list
template<typename T>
int linked_list<T>::size() {
	return _size;
}

//allocate a copy of our list and return a pointer to it
template<typename T>
T * linked_list<T>::list() {
	if (_size == 0)
		return (T*) NULL;
	T * new_list = (T*) malloc(sizeof(T) * _size);
	for (int i = 0; i < _size; i++) {
		new_list[i] = (*this)[i];
	}
	return new_list;
}

//clear out the entire linked list
template<typename T>
void linked_list<T>::clear() {
	while (_size > 0)
		this->pop();
}

//push an element on the front of the linked list
template<typename T>
void linked_list<T>::push(T data) {
	//reset last to -1 so [] will work right
	last = -1;
	
	//allocate a new node with the assigned data
	node<T> * tmp = new node<T>;
	tmp->data = data;
	tmp->next = NULL;
	
	//check if the list is empty
	if (_size == 0) {
		//if it is, set the top value to null
		top = tmp;
	} else {
		//otherwise, prepend this to before front
		tmp->next = top;
		top = tmp;
	}
	
	_size++;
}

//pop an element off the front of the linked list
template<typename T>
T linked_list<T>::pop(){
	//reset last to -1 so [] will work right
	last = -1;
	
	//hold a pointer to the start of the list
	node<T> * tmp = top;
	
	//advance the front of the list
	top = top->next;
	
	//check if it was the last value
	if (_size == 1) {
		top = NULL;
	}
	//store the copy of the data at the pointer
	//and delete the node
    T data_copy = tmp->data;
	
    delete(tmp);
	_size--;
    return data_copy;
}

//fetch an element from the list
template<typename T>
T linked_list<T>::operator[](int index) {
	assert(index < _size); //out of bounds error
	
	//check if the current index is the same as the last value fetched
	if (last == index) {
		return last_node->data;
	}
	
	//if last was -1, reset our current pointer to the start of the list
	//or if the requested index is before the last element we had
	if (last == -1 || index < last) {
		last = 0;
		last_node = top;
	}
	
	//now last is a valid index in our linked_list
	//and last_node is the corresponding node
	
	for (/*last = 0 or last index*/ ;last < index; last++) {
		last_node = last_node->next;
	}
	
	return last_node->data;
}

#endif
