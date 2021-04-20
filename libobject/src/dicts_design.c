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
    dict->vtable->erase(self);
    free(dict->contained);
    dict->contained = NULL;
    dict->contained_size = 0;
}

_class_definition(Dict, TYPE_DICT, dict_dtor)
