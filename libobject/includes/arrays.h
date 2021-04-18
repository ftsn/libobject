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

#define ARRAY   array_blueprint

// Class and constructor declaration
class_declaration(Array, ARRAY,
    Container   base;
    ssize_t     total_size;
)
ctor_declaration(Object *, ARRAY, Object *class; t_data **to_copy; ssize_t copy_amount;)

t_bool  array_alloc(Container *array, ssize_t size, t_array_opr operation, ...);

t_bool  _array_insert_at(Object *container, void *data, t_type type, ssize_t pos);
t_bool  _array_delete_at(Object *self, ssize_t pos);
t_bool  _array_erase(Object *self);

Object  *_array_front(const Object *self);
Object  *_array_back(const Object *self);
Object  *_array_at(const Object *self, ssize_t pos);

void    array_basic_print(ssize_t i, const t_data *elem, const char *prefix);

extern Class    *ARRAY;

#endif /* !ARRAYS_H_ */
