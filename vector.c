#include "vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool vector_init(vector* v, size_t stride) {
	if (!v || !stride) return false;
	v->length = 0;
	v->capacity = VECTOR_INITIAL_CAPACITY;
	v->stride = stride;
	v->data = malloc(stride * VECTOR_INITIAL_CAPACITY);
	return v->data != NULL;
}

vector* vector_new(size_t stride) {
	if (!stride) return NULL;
	vector* v = malloc(sizeof(vector));
	if (!v) return NULL;
	if (!vector_init(v, stride)) {
		free(v);
		return NULL;
	}
	return v;
}

bool vector_destroy(vector* v) {
	if (!v) return false;
	free(v->data);
	return true;
}

bool vector_resize(vector* v, size_t capacity) {
	if (!v || !v->data || capacity == 0) return false;
	void* data = realloc(v->data, v->stride * capacity);
	if (!data) return false;
	v->data = data;
	v->capacity = capacity;
	return true;
}

bool vector_insert(vector* v, size_t index, void* element) {
	if (!v || !v->data || index > v->length || !element) return false;
	if (v->length == v->capacity)
		if (!vector_resize(v, v->capacity * 2))
			return false;
	memmove((char*)v->data + v->stride * (index + 1), (char*)v->data + v->stride * index, v->stride * (v->length++ - index));
	memcpy((char*)v->data + v->stride * index, element, v->stride);
	return true;
}

bool vector_push_front(vector* v, void* element) {
	if (!v || !v->data || !element) return false;
	return vector_insert(v, 0, element);
}

bool vector_push_back(vector* v, void* element) {
	if (!v || !v->data || !element) return false;
	return vector_insert(v, v->length, element);
}

bool vector_remove(vector* v, size_t index) {
	if (!v || !v->data || index >= v->length) return false;
	memmove((char*)v->data + v->stride * index, (char*)v->data + v->stride * (index + 1), v->stride * (v->length-- - index));
	if (v->capacity == 4 * v->length)
		return vector_resize(v, v->capacity / 2);
	return true;
}

bool vector_pop_front(vector* v) {
	if (!v || !v->data) return false;
	return vector_remove(v, 0);
}
bool vector_pop_back(vector* v) {
	if (!v || !v->data) return false;
	return vector_remove(v, v->length - 1);
}

void* vector_at(vector* v, size_t index) {
	if (!v || index >= v->length) return NULL;
	return (char*)v->data + v->stride * index;
}

bool vector_replace(vector* v, size_t index, void* element) {
	if (!v || !v->data || index > v->length || !element) return false;
	memcpy((char*)v->data + v->stride * index, element, v->stride);
	return true;
}