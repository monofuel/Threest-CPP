#include <cstddef>
#include <cstdlib>
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
	T ** array;

public:
	vector();
	
	virtual void push(T);
	virtual int size();
	virtual T operator[](int);
};

template<typename T>
vector<T>::vector() {
	array = (T **) NULL;
	_size = 0;
}

template<typename T>
void vector<T>::push(T item) {
	if (array == NULL) {
		array = (T **) malloc(++_size * sizeof(void *));
	} else {
		array = (T **) realloc(array,++_size * sizeof(void *));
	}
	//TODO: do we have to allocate memory to item and make a copy of it?
	array[&item];
}

template<typename T>
int vector<T>::size() {
	return _size;
}

template<typename T>
T vector<T>::operator[](int index) {
	if (array == NULL || index >= _size) return NULL;
	
	return *array[index];

}

#endif
