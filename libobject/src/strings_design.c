#include <stdlib.h>
#include <string.h>
#include "arrays.h"
#include "stringsdef.h"
#include "types.h"

static t_bool   copy_ctor(String *string, char *copy, ssize_t size)
{
    char        *res;

    if (size == COPY_ALL)
        size = strlen(copy);
    if (!(res = calloc(size + 1, sizeof(char))))
        return (FALSE);
    memcpy(res, copy, size);
    string->contained = res;
    string->contained_size = size;
    return (TRUE);
}

Object      *ctor_definition(String) {
    String  *string;

    string = args->class;
    if (!string)
        return (NULL);
    if (args->to_copy)
        if (copy_ctor(string, args->to_copy, args->copy_amount) == FALSE)
        {
            delete(string);
            return (NULL);
        }
    return (string);
}

static void string_dtor(Object *self)
{
    String  *s;

    s = self;
    free(s->contained);
    s->contained = NULL;
}

_class_definition(String, TYPE_STRING, string_dtor)