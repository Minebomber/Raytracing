#pragma once

#include <stddef.h>
#include <stdbool.h>

#define VECTOR_INITIAL_CAPACITY 8

typedef struct vector {
	size_t length;
	size_t capacity;
	size_t stride;
	void* data;
} vector;

vector* vector_new(size_t stride);
bool vector_init(vector* v, size_t stride);
bool vector_destroy(vector* v);
bool vector_resize(vector* v, size_t capacity);

bool vector_insert(vector* v, size_t index, void* element);
bool vector_push_front(vector* v, void* element);
bool vector_push_back(vector* v, void* element);

bool vector_remove(vector* v, size_t index);
bool vector_pop_front(vector* v);
bool vector_pop_back(vector* v);

void* vector_at(vector* v, size_t index);
bool vector_replace(vector* v, size_t index, void* element);
