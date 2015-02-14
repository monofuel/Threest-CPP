//can't use these on arduino
//#include <cstddef>
//#include <cstdlib>
#ifdef __AVR_ARCH__
//suck it
#ifndef NULL
#define NULL null
#endif
#endif

#ifndef  LINKEDLIST_H
#define  LINKEDLIST_H

template<class T>
struct node {
	T data;
	node<T> * next;
};

template<typename T>
class linked_list {

protected:
	node<T> * top;
	node<T> * base;
	int _size;

public:
	linked_list();

	virtual int size();
	virtual void push(T);
	virtual void append(T);
	virtual T pop();
	virtual node<T>* get_top();

};

template<typename T>
linked_list<T>::linked_list() {
	top = (node<T> *) NULL;
	base = (node<T> *) NULL;
	_size = 0;
}

template<typename T>
node<T>* linked_list<T>::get_top(){
	return top;
}

template<typename T>
int linked_list<T>::size() {
	return _size;
}


template<typename T>
void linked_list<T>::append(T data) {
	node<T> * tmp = new node<T>;
	tmp->data = data;
	tmp->next = NULL;
	if (base != NULL) base->next = tmp;
	else top = tmp;
	base = tmp;
	_size++;
}

template<typename T>
void linked_list<T>::push(T data){
	node<T> * tmp = new node<T>;
	tmp->next = top;
	tmp->data = data;
	if (top == NULL) base = tmp;
	top = tmp;
	_size++;
}

template<typename T>
T linked_list<T>::pop(){
	_size--;
	node<T> * tmp = top;
	top = top->next;
	if (top == NULL) base = NULL;
    T data_copy = tmp->data;
    delete(tmp);
    return data_copy;
}
#endif
