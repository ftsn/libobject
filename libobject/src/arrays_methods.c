#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

static void copy_performs_addition(void **dest, void **src, t_data *typed_data, ssize_t pos, ssize_t total_size)
{
    ssize_t i;
    ssize_t j;

    i = -1;
    if (!dest || !src)
        return;
    while (++i < pos)
        dest[i] = src[i];
    j = i;
    dest[i++] = typed_data;
    while (j < total_size)
        dest[i++] = src[j++];
}

static void     copy_performs_deletion(void **dest, void **src, ssize_t pos, ssize_t total_size)
{
    ssize_t     i;
    ssize_t     j;

    i = -1;
    if (!dest || !src)
        return;
    while (++i < pos)
        dest[i] = src[i];
    j = i;
    ++i;
    while (j < total_size)
        dest[j++] = src[i++];
}

t_bool      array_alloc(Container *array, ssize_t size, t_array_opr operation, ...)
{
    ssize_t new_size;
    ssize_t i;
    void    **contained;
    va_list ap;

    (void)size;
    va_start(ap, operation);
    if (operation == ADDITION)
        new_size = ARRAY_ALLOC_SIZE(array->contained_size + 1);
    else if (operation == DELETION)
        new_size = ARRAY_ALLOC_SIZE(array->contained_size - 1);
    else
        new_size = ARRAY_ALLOC_SIZE(array->contained_size);
    if (!(contained = malloc(sizeof(void *) * (new_size + 1))))
        return (FALSE);
    contained[new_size] = NULL;
    ((Array *)array)->total_size = new_size;
    if (operation == ADDITION)
    {
        array->contained_size += 1;
        copy_performs_addition(contained, array->contained, va_arg(ap, t_data *), va_arg(ap, ssize_t), array->contained_size);
    }
    else if (operation == DELETION)
    {
        array->contained_size -= 1;
        copy_performs_deletion(contained, array->contained, va_arg(ap, ssize_t), array->contained_size);
    }
    i = array->contained_size - 1;
    while (++i < new_size)
        contained[i] = NULL;
    free(array->contained);
    array->contained = contained;
    va_end(ap);
    return (TRUE);
}

t_bool          _array_insert_at(Object *container, void *data, t_type type, ssize_t pos)
{
    Container   *self;
    t_data      *typed_data;

    self = container;
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (array_alloc(self, 666, ADDITION, typed_data, pos) == FALSE)
    {
        free(typed_data);
        return (FALSE);
    }
    return (TRUE);
}

t_bool          _array_delete_at(Object *container, ssize_t pos)
{
    Container   *self;

    self = container;
    if (self->contained_size == 0 || pos >= self->contained_size)
        return (FALSE);
    if (array_alloc(self, 666, DELETION, pos) == FALSE)
        return (FALSE);
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
