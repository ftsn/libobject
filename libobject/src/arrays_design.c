#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

static t_bool   _array_ctor(Object *self, va_list *args)
{
    Container   *array;
    ssize_t     nb_args;
    void        *copy;

    array = self;
    if (array_alloc(array, ARRAY_ALLOC_SIZE(array->contained_size), ARRAY_NO_OPERATION) == FALSE)
        return (FALSE);
    if ((copy = va_arg(*args, void *)))
        if (ctn_copy_ctor(array, copy, va_arg(*args, ssize_t)) == FALSE)
            return (FALSE);
    nb_args = va_arg(*args, ssize_t);
    while (nb_args > 0)
    {
        if (array->push_back(array, va_arg(*args, void *), va_arg(*args, t_type)) == FALSE)
            return (FALSE);
        --nb_args;
    }
    return (TRUE);
}

static void     _array_dtor(Object *self, va_list *args)
{
    Iterator    *it;

    if ((it = ((Container *)self)->begin(self)) == NULL)
        return ;
    while (!it->reached_the_end)
    {
        free(it->dereference(it));
        it->next(it);
    }
    delete(it);
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
                &_array_ctor,
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
