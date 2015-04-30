#include <cstddef>
#include <cstdlib>
#include <cassert>
#ifdef __AVR_ARCH__
//suck it
#ifndef NULL
#define NULL null
#endif
#endif

#ifndef	VECTOR_H
#define VECTOR_H

template<typename T>
class vector {

protected:
	int _size;
	T * array;

public:
	vector();
	vector(T*,int);
	~vector();
	
	virtual void push(T);
	virtual void clear();
	virtual int size();
	virtual T* list();
	virtual T operator[](int);
	vector<T>& operator=(const vector<T>);
};

template<typename T>
vector<T>::vector() {
	array = (T *) NULL;
	_size = 0;
}

template<typename T>
vector<T>::vector(T* list, int size) {
	array = (T *) malloc(size * sizeof(T));
	assert(array != NULL);
	_size = size;
	memcpy(array,list,size * sizeof(T));
}

template<typename T>
vector<T>::~vector() {
	free(array);
	_size = 0;
}

template<typename T>
void vector<T>::push(T item) {
	T* newArray = NULL;
	
	//check if we are initializing a new array, or creating a new one
	if (_size == 0) {
		array = (T *) malloc(++_size * sizeof(T));
		assert(array != NULL); //failed to allocate memory
	} else {
		newArray = (T *) realloc(array,++_size * sizeof(T));
		assert(newArray != NULL); //failed to allocate memory
		array = newArray;
	}

	array[_size - 1] = item;
}

template<typename T>
int vector<T>::size() {
	return _size;
}

template<typename T>
void vector<T>::clear() {
	if (_size == 0) return;
	free(array);
	_size = 0;
}

template<typename T>
T* vector<T>::list() {
	T* output_array = (T *) malloc(_size * sizeof(T));
	assert(output_array != NULL);
	memcpy(output_array,array,_size * sizeof(T));
}

template<typename T>
T vector<T>::operator[](int index) {
	assert(index < _size); //out of bounds error
	
	return array[index];
}

template<typename T>
vector<T>& vector<T>::operator=( const vector<T> other) {
	//TODO not sure what happens if you assign = to an existing vector
	//does it destruct the previous vec and run this, or
	//does it just run this on the existing vec?
	array = other.list();
	_size = other.size();
}

#endif
