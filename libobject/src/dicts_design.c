#include <stdlib.h>
#include "dicts.h"
#include "arrays.h"

static      t_bool copy_ctor(Container *dict, void **copy, ssize_t size)
{
    ssize_t i;

    i = 0;
    if (size == COPY_ALL)
    {
        size = 0;
        while (copy[size])
            ++size;
    }
    while (i < size)
    {
        if (dict->push_back(dict, copy[i]) == FALSE)
            return (FALSE);
        ++i;
    }
    return (TRUE);
}

static t_bool   _dict_ctor(Object *self, va_list *args)
{
    Container   *dict;
    ssize_t     nb_args;
    void        *copy;

    dict = self;
    if (!(dict->contained = calloc(1, sizeof(void *))))
        return (FALSE);
    if ((copy = va_arg(*args, void *)))
        if (copy_ctor(dict, copy, va_arg(*args, ssize_t)) == FALSE)
            return (FALSE);
    nb_args = va_arg(*args, ssize_t);
    while (nb_args > 0)
    {
        if (dict->push_back(dict, va_arg(*args, void *)) == FALSE)
            return (FALSE);
        --nb_args;
    }
    return (TRUE);
}

static void _dict_dtor(Object *self, va_list *args)
{
    (void)args;
    free(((Container *)self)->contained);
    ((Container *)self)->contained = NULL;
    ((Container *)self)->contained_size = 0;
}

static Dict _dict_descr =
    {
        {
            {
                TYPE_DICT,
                sizeof(Dict),
                &_dict_ctor,
                &_dict_dtor
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
            &_container_last
        },
        &_get_obj_by_key,
        &_dict_push_back
    };

Class   *_dict = (Class *)&_dict_descr;
