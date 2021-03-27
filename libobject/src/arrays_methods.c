#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

t_bool          _array_delete_at(Object *container, ssize_t pos)
{
    Container   *self;
    void        **res;
    ssize_t     i;
    ssize_t     j;

    self = container;
    if (self->empty(self) == TRUE)
        return (FALSE);
    if (pos >= self->contained_size)
        pos = self->contained_size - 1;
    if (!(res = malloc(self->contained_size * sizeof(void *))))
        return (FALSE);
    i = -1;
    while (((void **)self->contained)[++i] && i < pos)
        res[i] = ((void **)self->contained)[i];
    j = i;
    ++i;
    while (((void **)self->contained)[i])
        res[j++] = ((void **)self->contained)[i++];
    res[j] = NULL;
    free(self->contained);
    self->contained = res;
    --self->contained_size;
    return (TRUE);
}

t_bool          _array_erase(Object *container)
{
    Container   *self;

    self = container;
    if (self->empty(self) == TRUE || self->delete_at(self, 0) == FALSE)
        return (FALSE);
    self->erase(self);
    return (TRUE);
}

Object              *_array_front(const Object *array)
{
    const Container *container;

    container = array;
    return (container->contained ? ((void **)container->contained)[0] : NULL);
}

Object              *_array_back(const Object *array)
{
    const Container *container;

    container = array;
    return (container->contained ? ((void **)container->contained)[container->contained_size - 1] : NULL);
}

Object              *_array_at(const Object *self, ssize_t pos)
{
    const Container *container;

    container = self;
    return (pos < container->contained_size && container->contained ? ((void **)container->contained)[pos] : NULL);
}

/*
** Basic function provided to the user to allow him to print the container
** without having to create his own function
*/
void    array_basic_print(ssize_t i, const t_data *elem, const char *prefix)
{
    switch (elem->type)
    {
    case TYPE_CHAR:
        printf("%s%d: [%c]\n", prefix, (int)i, *(char *)elem->data);
        break;
    case TYPE_CSTRING:
        printf("%s%d: [%s]\n", prefix, (int)i, (char *)elem->data);
        break;
    case TYPE_INT:
        printf("%s%d: [%d]\n", prefix, (int)i, *(int *)elem->data);
        break;
    default:;
    }
}
