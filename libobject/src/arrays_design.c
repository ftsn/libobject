#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

Object      *ctor_definition(Array) {
    Array   *array;

    array = args->class;
    if (!array)
        return (NULL);
    if (array_alloc(array, ARRAY_ALLOC_SIZE(array->contained_size), ARRAY_NO_OPERATION) == FALSE)
    {
        delete(array);
        return (NULL);
    }
    if (args->to_copy)
        if (ctn_copy_ctor((Container *)array, args->to_copy, args->copy_amount) == FALSE)
        {
            delete(array);
            return (NULL);
        }
    return (array);
}

static void     array_dtor(Object *self)
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
}

_class_definition(Array, TYPE_ARRAY, array_dtor)