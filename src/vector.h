//can't use any of these on arduino
//#include <cstddef>
//#include <cstdlib>
//#include <cassert>
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
	virtual void clear();
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
    array = (T **) realloc(array,++_size * sizeof(void *));
	T * item_copy = (T *) malloc(sizeof(T));
	*item_copy = item;

	array[_size - 1] = item_copy;
}

template<typename T>
int vector<T>::size() {
	return _size;
}

template<typename T>
void vector<T>::clear() {
	if (_size == 0) return;
	for (int i = 0; i < _size; i++) {

		free(array[i]);
	}
	free(array);
	_size = 0;
}

template<typename T>
T vector<T>::operator[](int index) {
	//assert(index < _size); //out of bounds error
	if (index >= _size) {
		exit(1);
	}
	
	//TODO find a nicer way to handle this
	/*if (array == NULL || index >= _size) {
		return NULL;
	}*/
	
	return *(array[index]);

}

#endif
