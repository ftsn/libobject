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
    free(((Container *)self)->contained);
    ((Container *)self)->contained = NULL;
}

class_definition(String,
    class_metadata(String, TYPE_STRING, string_dtor),
    NULL,
    0,

    &_container_data,
    &_container_size,
    &_container_empty,

    &_string_insert_at,
    &_string_push_back,
    &_string_delete_at,
    &_string_erase,
    &_string_affect,

    &_string_front,
    &_string_back,
    &_string_at,

    &_string_print,

    &_string_begin,
    &_string_end,

    NULL,
    &_string_findstr,
    &_string_find,
    &_string_lfind,
    &_string_match,
    &_string_nmatch,
    &_string_split,
    &_string_sub
)