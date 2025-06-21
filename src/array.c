//
// Created by luis on 6/21/25.
//

#include "array.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define ARRAY_DEFAULT_CAPACITY 64

typedef struct array_t {
    uint32_t size;
    uint32_t length;
    uint32_t capacity;
    char *array;
} array_t;

array_t *array_new(uint32_t size) {

    array_t *a = malloc(sizeof(array_t));
    if (a == NULL)
       return NULL;

    a->size = size;
    a->length = 0;
    a->capacity = ARRAY_DEFAULT_CAPACITY;
    a->array = malloc(a->size * a->capacity);
    if (a->array == NULL) {
        free(a);
        a = NULL;
    }

    return a;
}

void array_free(array_t *a) {
    assert(a != NULL);
    free(a->array);
    a->array = NULL;
    free(a);
}

int32_t array_resize(array_t *a, uint32_t length) {
    assert(a != NULL);
    assert(length >= a->length);

    a->length = length;
    a->capacity = a->length;
    a->array = realloc(a->array, a->capacity * a->size);
    if (a->array == NULL) {
        free(a);
        return -1;
    }

    return 0;
}

uint32_t array_length(array_t *a) {
    assert(a != NULL);
    return a->length;
}

char * array_at(array_t *a, uint32_t index) {
    assert(a != NULL);
    assert(index < a->length);

    return a->array + index * a->size;
}

void array_set(array_t *a, uint32_t index, void *element) {
    assert(a != NULL);
    assert(element != NULL);
    assert(index < a->length);

    memcpy(a->array + index * a->size, element, a->size);
}

void array_push(array_t *a, void *element) {
    assert(a != NULL);
    assert(element != NULL);

     if (a->length >= a->capacity) {
         a->capacity *= 2;
         a->array = realloc(a->array, a->capacity * a->size);
     }
     a->length++;
     array_set(a, a->length - 1, element);
}

void array_pop(array_t *a, void *element) {
    assert(a != NULL);
    assert(element != NULL);
    assert(a->length > 0);

    memcpy(element, a->array + a->length * a->size, a->size);
    a->length--;
}

void *array_raw(array_t *a) {
    assert(a != NULL);
    return a->array;
}