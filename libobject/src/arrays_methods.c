#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

static void copy_performs_addition(void **dest, void **src, t_data *typed_data, ssize_t pos, ssize_t total_size)
{
    ssize_t i;
    ssize_t j;

    i = 0;
    if (!dest || !src)
        return;
    if (dest != src)
    {
        while (i < pos)
        {
            dest[i] = src[i];
            ++i;
        }
        j = i;
        dest[i++] = typed_data;
        while (j < total_size)
            dest[i++] = src[j++];
    }
    else
    {
        i = total_size;
        while (--i > pos)
            dest[i + 1] = dest[i];
        dest[pos] = typed_data;
    }
}

static void     copy_performs_deletion(void **dest, void **src, ssize_t pos, ssize_t total_size)
{
    ssize_t     i;
    ssize_t     j;

    i = 0;
    if (!dest || !src)
        return;
    if (dest != src)
    {
        while (i < pos)
        {
            dest[i] = src[i];
            ++i;
        }
        free(dest[pos]);
        j = i;
        ++i;
        while (j < total_size)
            dest[j++] = src[i++];
        dest[j] = NULL;
    }
    else
    {
        free(dest[pos]);
        i = j = pos;
        ++i;
        while (j < total_size)
            dest[j++] = src[i++];
        dest[j] = NULL;
    }
}

t_bool      array_alloc(Container *array, ssize_t new_size, t_array_opr operation, ...)
{
    ssize_t i;
    void    **contained;
    t_bool  same_dest_src;
    va_list ap;

    contained = NULL;
    va_start(ap, operation);
    if ((operation == ARRAY_ADDITION && array->contained_size < ((Array *)array)->total_size) ||
        (operation == ARRAY_DELETION && array->contained_size % CHUNK_SIZE))
    {
        same_dest_src = TRUE;
        contained = array->contained;
    }
    else
    {
        same_dest_src = FALSE;
        if (!(contained = malloc(sizeof(void *) * (new_size + 1))))
            return (FALSE);
        contained[new_size] = NULL;
        ((Array *)array)->total_size = new_size;
    }
    if (operation == ARRAY_ADDITION)
    {
        array->contained_size += 1;
        copy_performs_addition(contained, array->contained, va_arg(ap, t_data *), va_arg(ap, ssize_t), array->contained_size);
    }
    else if (operation == ARRAY_DELETION)
    {
        array->contained_size -= 1;
        copy_performs_deletion(contained, array->contained, va_arg(ap, ssize_t), array->contained_size);
    }
    if (same_dest_src == FALSE)
    {
        if (operation == ARRAY_NO_OPERATION)
        {
            i = -1;
            while (++i < array->contained_size)
                contained[i] = ((void **)array->contained)[i];
        }
        i = array->contained_size - 1;
        while (++i < new_size)
            contained[i] = NULL;
        free(array->contained);
        array->contained = contained;
    }
    va_end(ap);
    return (TRUE);
}

t_bool          _array_insert_at(Object *container, void *data, t_type type, ssize_t pos)
{
    Container   *self;
    t_data      *typed_data;

    self = container;
    if (pos < 0 || pos > self->contained_size)
        return (FALSE);
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (array_alloc(self, ARRAY_ALLOC_SIZE(self->contained_size + 1), ARRAY_ADDITION, typed_data, pos) == FALSE)
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
    if (self->contained_size <= 0 || pos >= self->contained_size)
        return (FALSE);
    if (array_alloc(self, ARRAY_ALLOC_SIZE(self->contained_size - 1), ARRAY_DELETION, pos) == FALSE)
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
