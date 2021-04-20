#ifndef ARRAYS_H_
# define ARRAYS_H_

# include "containers.h"

# define CHUNK_SIZE 50
# define ARRAY_ALLOC_SIZE(s) (((s) / CHUNK_SIZE + 1) * CHUNK_SIZE)

typedef enum e_array_opr
{
    ARRAY_NO_OPERATION = 0,
    ARRAY_ADDITION,
    ARRAY_DELETION,
} t_array_opr;

// Class and constructor declaration
#define Array_fields        \
        Container_fields    \
        ssize_t total_size;
#define Array_definition                \
        Container_definition,           \
        .insert_at = _array_insert_at,  \
        .delete_at = _array_delete_at,  \
        .erase = _array_erase,          \
        .front = _array_front,          \
        .back = _array_back,            \
        .at = _array_at,                \
        .total_size = 0

class_declaration(Array)
ctor_declaration(Object *, Array, Object *class; t_data **to_copy; ssize_t copy_amount;)

t_bool  array_alloc(Array *array, ssize_t size, t_array_opr operation, ...);

t_bool  _array_insert_at(Object *container, void *data, t_type type, ssize_t pos);
t_bool  _array_delete_at(Object *self, ssize_t pos);
t_bool  _array_erase(Object *self);

Object  *_array_front(const Object *self);
Object  *_array_back(const Object *self);
Object  *_array_at(const Object *self, ssize_t pos);

#endif /* !ARRAYS_H_ */
