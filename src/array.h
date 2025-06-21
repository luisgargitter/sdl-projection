#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>

typedef struct array_t array_t;

array_t *array_new(uint32_t size);
void array_free(array_t *array);
int32_t array_resize(array_t *array, uint32_t capacity);
uint32_t array_length(array_t *array);
char * array_at(array_t *array, uint32_t index);
void array_set(array_t *array, uint32_t index, void *element);
void array_push(array_t *array, void *element);
void array_pop(array_t *array, void *element);
void * array_raw(array_t *array);

#endif //ARRAY_H