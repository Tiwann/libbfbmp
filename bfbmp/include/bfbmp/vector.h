#pragma once
#include "def.h"

#define BFBMP_DECLARE_TEMPLATE_VECTOR_STRUCT(T, Name) \
typedef struct __bfbmp_vector_##Name \
{ \
    T* data; \
    size_t allocated; \
    size_t count; \
} bfbmp_vector_##Name##_t; \


#define BFBMP_IMPL_TEMPLATE_VECTOR_FUNCTIONS_STATIC(T, Name) \
BFBMP_PRIVATE bfbmp_vector_##Name##_t bfbmp_vector_##Name##_create(void) \
{ \
    bfbmp_vector_##Name##_t vec; \
    vec.count = 0; \
    vec.allocated = 1; \
    vec.data = (T*)malloc(vec.allocated * sizeof(T)); \
    return vec;\
} \
\
BFBMP_PRIVATE uint8_t bfbmp_vector_##Name##_push(bfbmp_vector_##Name##_t* vector, T element) \
{ \
    BFBMP_CHECK(vector); \
    BFBMP_CHECK(vector->data); \
    if(vector->count == vector->allocated) \
    { \
        vector->allocated *= 2; \
        T* ptr = (T*)realloc(vector->data, vector->allocated * sizeof(T)); \
        BFBMP_CHECK(ptr); \
        vector->data = ptr; \
    } \
    \
    vector->data[vector->count] = element; \
    vector->count++; \
    return BFBMP_TRUE; \
} \
\
BFBMP_PRIVATE void bfbmp_vector_##Name##_free(bfbmp_vector_##Name##_t* vector) \
{ \
    free(vector->data); \
    vector->allocated = 0; \
    vector->count = 0; \
} \
\
BFBMP_PRIVATE uint8_t bfbmp_vector_##Name##_pop(bfbmp_vector_##Name##_t* vector) \
{ \
    BFBMP_CHECK(vector);\
    if(vector->count <= 0) return BFBMP_FALSE; \
    memset(vector->data + (vector->count - 1), 0, sizeof(T)); \
    vector->count--; \
    return BFBMP_TRUE; \
}

