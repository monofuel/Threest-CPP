//can't use any of these on arduino
//#include <cstddef>
//#include <cstdlib>
//#include <cassert>
#ifdef __AVR_ARCH__
#include <assert.h>
//suck it
#ifndef NULL
#define NULL null
#endif

#else //if not on arduino
#include <cassert>
#include <cstring>

#endif

#ifndef	vec_H
#define vec_H

#ifndef DEBUG
#define DEBUG true
#endif


template<typename T>
class vec {

protected:
	int _size;
	T * array;

public:
	vec();
	vec(T*,int);
	vec(const vec<T>&);
	~vec();
	
	void push(T);
	T pop();
	void clear();
	int size();
	T* list();
	T operator[](int);
	
};

//instantiate a new vec without any elements
template<typename T>
vec<T>::vec() {
	array = (T *) NULL;
	_size = 0;
}

//instantiate a new vec with a given list and _size
template<typename T>
vec<T>::vec(T* list, int size) {
	array = (T *) malloc(size * sizeof(T));
	if (DEBUG) assert(array != NULL);
	_size = size;
	memcpy(array,list,size * sizeof(T));
}

//copy constructor
template<typename T>
vec<T>::vec( const vec<T> &other) {

	array = (T *) malloc(other._size * sizeof(T));
	if (DEBUG) assert(array != NULL);
	memcpy(array,other.array,other._size * sizeof(T));
	_size = other._size;
}

//destructor
template<typename T>
vec<T>::~vec() {
	if (_size > 0)
		free(array);
	_size = 0;
}

//push a new element onto the end of the vec
template<typename T>
void vec<T>::push(T item) {
	T* newArray = NULL;
	
	//on first element, array will be NULL
	//check if we are initializing a new array, or creating a new one
	newArray = (T *) realloc(array,++_size * sizeof(T));
	if (DEBUG) assert(newArray != NULL); //failed to allocate memory
	array = newArray;

	array[_size - 1] = item;
}

//pop an element off the top of the vec
template<typename T>
T vec<T>::pop() {
	T* newArray = NULL;
	
	T tmp = array[_size-1];
	newArray = (T *) realloc(array,--_size * sizeof(T));
	if (DEBUG) {
		if (_size > 0)
			assert(newArray != NULL); //failed to allocate memory
	}
	array = newArray;
	return tmp;
}

//get the _size of the vec
template<typename T>
int vec<T>::size() {
	return _size;
}

//clear out the vec
template<typename T>
void vec<T>::clear() {
	if (_size == 0) return;
	free(array);
	_size = 0;
}

//allocate a copy of the list and return a pointer to it
template<typename T>
T* vec<T>::list() {
	if (DEBUG) {
		if (_size == 0)
			return (T*) NULL; //if _size is 0, pretend this didn't happen
	}
	T* output_array = (T *) malloc(_size * sizeof(T));
	if (DEBUG) assert(output_array != NULL);
	memcpy(output_array,array,_size * sizeof(T));
	return output_array;
}

//fetch an element from the list
template<typename T>
T vec<T>::operator[](int index) {
	if (DEBUG) assert(index < _size); //out of bounds error
	
	
	return array[index];
}

#endif
