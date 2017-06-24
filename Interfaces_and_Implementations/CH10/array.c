#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "../CH5/mem.h"


#define T Array_T

// Initializes and sets up the array, either creates an array of length size, or an empty array
T Array_new(int length, int size) {
	T array;

	NEW(array);

	if(length > 0)
		ArrayRep_init(array, length, size, CALLOC(length, size));
	else
		ArrayRep_init(array, length, size, NULL);
	return array;
}

// Function which provides direct access to the underlying structure and allows construction of arrays from any allocated block of memeory
void ArrayRep_init(T array, int length, int size, void *ary) {
	assert(array);
	assert(ary && length > 0 || length == 0 && ary == NULL);
	assert(size > 0);
	array->length = length;
	array->size = size;
	if(length > 0)
		array->array = ary;
	else 
		array->array = NULL;
}


void Array_free(T *array) {
	assert(array & *array);
	FREE((*array)->array);
	FREE(*array);
}


void *Array_get(T array, int i) {
	assert(array);
	assert(i >= 0 && i < array->length);
	return array->array + i * array->size;
}

void *Array_put(T array, int i, void *elem) {
	assert(array);
	assert(i >= 0 && i < array->length);
	assert(elem);
	memcpy(arra->array + i*array->size, elem, array->size);
	return elem;
}

int Array_length(T array) {
	assert(array);
	return array->length;
}

int Array_size(T array) {
	assert(array);
	return array->size;
}

void Array_resize(T array, int length) {
	assert(array);
	assert(length >= 0);

	if(length == 0)
		FREE(array->array);
	else if(array->length == 0)
		array->array = ALLOC(length * array->size);
	else 
		RESIZE(array->array, length * array->size);
	array->length = length;
}


// Also handles copying a subsequence of the array
T Array_copy(T array, int length) {
	T copy;
	assert(array);
	assert(length >= 0);
	copy = Array_new(length, array->size);
	if(copy->length >= array->length && array->length > 0)
		memcpy(copy->array, array->array, array->length * array->size);
	else if(array->length > copy->length && copy->length > 0)
		memcpy(copy->array, array->array, copy->length * array->size);
	return copy;
}

