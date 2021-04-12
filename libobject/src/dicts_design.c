#include <stdlib.h>
#include <stdio.h>
#include "dicts.h"
#include "arrays.h"
#include "iterators.h"
#include "lists.h"

Object          *variadic_func_definition(dict_ctor) {
    Container   *dict;

    dict = args->class;
    if (!dict)
        return (NULL);
    if (dict_alloc(dict, DICT_ALLOC_SIZE(dict->contained_size)) == FALSE)
    {
        delete(dict);
        return (NULL);
    }
    return (dict);
}

static Object   *_shallow_dict_ctor()
{
    return (new_obj(DICT));
}

static void     _dict_dtor(Object *self, va_list *args)
{
    ((Container *)self)->erase(self);
    free(((Container *)self)->contained);
    ((Container *)self)->contained = NULL;
    ((Container *)self)->contained_size = 0;
    (void)args;
}

static Dict _dict_descr =
    {
        {
            {
                TYPE_DICT,
                sizeof(Dict),
                &_shallow_dict_ctor,
                &_dict_dtor
            },
            NULL,
            0,

            &_container_data,
            &_container_size,
            &_container_empty,

            NULL, // insert_at
            NULL, // push_back
            NULL, // delete_at
            &_dict_erase, // erase
            NULL,

            NULL, // front
            NULL, // back
            NULL, // at

            &_container_print,

            NULL,
            NULL,
            NULL,

            &_container_begin,
            &_container_end
        },
        0,
        &_get_obj_by_key,
        &_dict_push,
        &_dict_remove,
    };

Class   *_dict = (Class *)&_dict_descr;
