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
#define Array_fields            \
        Container_fields        \
        ssize_t total_size;
#define Array_vtable_fields     \
        Container_vtable_fields

#define Array_definition          		\
        Container_definition,           \
        .total_size = 0
#define Array_vtable_definition         \
        Container_vtable_definition,    \
        .insert_at = array_insert_at,   \
        .delete_at = array_delete_at,   \
        .erase = array_erase,           \
        .front = array_front,           \
        .back = array_back,             \
        .at = array_at

class_declaration(Array)
ctor_declaration(Object *, Array, Object *class; t_data **to_copy; ssize_t copy_amount;)

t_bool  array_alloc(Array *array, ssize_t size, t_array_opr operation, ...);

t_bool  array_insert_at(Object *container, void *data, t_type type, ssize_t pos);
t_bool  array_delete_at(Object *self, ssize_t pos);
t_bool  array_erase(Object *self);

Object  *array_front(const Object *self);
Object  *array_back(const Object *self);
Object  *array_at(const Object *self, ssize_t pos);

#endif /* !ARRAYS_H_ */
