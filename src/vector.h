//can't use any of these on arduino
//#include <cstddef>
//#include <cstdlib>
//#include <cassert>
#include <cstring>
#ifdef __AVR_ARCH__
#include <assert.h>
//suck it
#ifndef NULL
#define NULL null
#endif

#else
#include <cassert>

#endif

#ifndef	vec_H
#define vec_H

template<typename T>
class vec {

protected:
	int _size;
	T * array;

public:
	vec();
	vec(T*,int);
	virtual ~vec();
	
	virtual void push(T);
	virtual T pop();
	virtual void clear();
	virtual int size();
	virtual T* list();
	virtual T operator[](int);
	vec<T>& operator=(const vec<T>);
};

template<typename T>
vec<T>::vec() {
	array = (T *) NULL;
	_size = 0;
}

template<typename T>
vec<T>::vec(T* list, int size) {
	array = (T *) malloc(size * sizeof(T));
	assert(array != NULL);
	_size = size;
	memcpy(array,list,size * sizeof(T)); //TODO verify memcpy on arduino
}

template<typename T>
vec<T>::~vec() {
	if (_size > 0)
		free(array);
	_size = 0;
}

template<typename T>
void vec<T>::push(T item) {
	T* newArray = NULL;
	
	//on first element, array will be NULL
	//check if we are initializing a new array, or creating a new one
	newArray = (T *) realloc(array,++_size * sizeof(T));
	assert(newArray != NULL); //failed to allocate memory
	array = newArray;

	array[_size - 1] = item;
}

template<typename T>
T vec<T>::pop() {
	T* newArray = NULL;
	
	T tmp = array[_size-1];
	newArray = (T *) realloc(array,--_size * sizeof(T));
	if (_size != 0) //if we don't expect it to be null
		assert(newArray != NULL); //check if failed to allocate memory
	array = newArray;
	return tmp;
}

template<typename T>
int vec<T>::size() {
	return _size;
}

template<typename T>
void vec<T>::clear() {
	if (_size == 0) return;
	free(array);
	_size = 0;
}

/*
 * get a copy of the current list of elements.
 * will return null if there are no elements.
 */
template<typename T>
T* vec<T>::list() {
	if (_size == 0) {
		//lets uhh, return null and pretend this didn't happen
		return (T*) NULL;
	}
	
	T* output_array = (T *) malloc(_size * sizeof(T));
	assert(output_array != NULL);
	memcpy(output_array,array,_size * sizeof(T)); //TODO verify memcpy on arduino
	return output_array;
}

template<typename T>
T vec<T>::operator[](int index) {
	assert(index < _size); //out of bounds error
	
	
	return array[index];
}

template<typename T>
vec<T>& vec<T>::operator=( const vec<T> other) {
	//TODO not sure what happens if you assign = to an existing vec
	//does it destruct the previous vec and run this, or
	//does it just run this on the existing vec?
	std::cout << "copying list" << std::endl;
	array = other.list();
	std::cout << "copying size" << std::endl;
	_size = other.size();
	std::cout << "copy done" << std::endl;
}

#endif
