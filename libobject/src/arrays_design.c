#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

Object          *variadic_func_definition(array_ctor) {
    Container   *array;

    array = args->class;
    if (!array)
        return (NULL);
    if (array_alloc(array, ARRAY_ALLOC_SIZE(array->contained_size), ARRAY_NO_OPERATION) == FALSE)
    {
        delete(array);
        return (NULL);
    }
    if (args->to_copy)
        if (ctn_copy_ctor(array, args->to_copy, args->copy_amount) == FALSE)
        {
            delete(array);
            return (NULL);
        }
    return (array);
}

static Object   *_shallow_array_ctor()
{
    return (new_obj(ARRAY));
}

static void     _array_dtor(Object *self, va_list *args)
{
    ssize_t     i;
    void        **contained;

    contained = ((Container *)self)->contained;
    i = -1;
    if (contained)
    {
        while (contained[++i])
        {
            free(contained[i]);
            contained[i] = NULL;
        }
    }
    free(((Container *)self)->contained);
    ((Container *)self)->contained = NULL;
    ((Container *)self)->contained_size = 0;
    (void)args;
}

static Array _array_descr =
    {
        {
            {
                TYPE_ARRAY,
                sizeof(Array),
                &_shallow_array_ctor,
                &_array_dtor
            },
            NULL,
            0,

            &_container_data,
            &_container_size,
            &_container_empty,

            &_array_insert_at,
            &_container_push_back,
            &_array_delete_at,
            &_array_erase,
            NULL,

            &_array_front,
            &_array_back,
            &_array_at,

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_end,
        },
        0
    };

Class *_array = (Class *)&_array_descr;
