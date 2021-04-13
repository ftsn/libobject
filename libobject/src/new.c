#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "object.h"
#include "utils.h"

Object  *shallow_new_obj(const Class *class, ...)
{
    return (class->__init__());
}

void        delete(Object *ptr, ...)
{
    Class   *class;
    va_list ap;

    if (!ptr)
        return;
    va_start(ap, ptr);
    class = ptr;
    class->__del__(ptr, &ap);
    va_end(ap);
    free(ptr);
}

Object      *_init_new_obj(const Class *class)
{
    Object  *new;

    new = malloc(class->__size__);
    if (!new)
        return (NULL);
    new = memcpy(new, class, class->__size__);
    return (new);
}