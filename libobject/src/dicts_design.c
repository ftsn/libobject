#include <stdlib.h>
#include <stdio.h>
#include "dicts.h"
#include "arrays.h"
#include "iterators.h"
#include "lists.h"

Object      *ctor_definition(Dict) {
    Dict    *dict;

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

static void dict_dtor(Object *self)
{
    Dict    *dict;

    dict = self;
    dict->erase(self);
    free(dict->contained);
    dict->contained = NULL;
    dict->contained_size = 0;
}

class_definition(Dict,
    class_metadata(Dict, TYPE_DICT, dict_dtor),
    NULL,
    0,

    _container_data,
    _container_size,
    _container_empty,

    NULL, // insert_at
    NULL, // push_back
    NULL, // delete_at
    _dict_erase, // erase
    NULL,

    NULL, // front
    NULL, // back
    NULL, // at

    _container_print,

    NULL,
    NULL,
    NULL,

    _container_begin,
    _container_end,
    0,
    _get_obj_by_key,
    _dict_push,
    _dict_remove,
)
