#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

static void copy_performs_addition(void **dest, void **src, t_data *typed_data, ssize_t pos, ssize_t ctn_size)
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
        while (j < ctn_size)
            dest[i++] = src[j++];
    }
    else
    {
        // We are getting ctn_size + 1 in arg and ctn_size starts at 1 when non empty
        i = ctn_size - 2;
        while (i >= pos)
        {
            dest[i + 1] = dest[i];
            --i;
        }
        dest[pos] = typed_data;
    }
}

static void     copy_performs_deletion(void **dest, void **src, ssize_t pos, ssize_t ctn_size)
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
        while (j < ctn_size)
            dest[j++] = src[i++];
        dest[j] = NULL;
    }
    else
    {
        free(dest[pos]);
        i = j = pos;
        ++i;
        while (j < ctn_size)
            dest[j++] = src[i++];
        dest[j] = NULL;
    }
}

t_bool      array_alloc(Array *array, ssize_t new_size, t_array_opr operation, ...)
{
    ssize_t i;
    void    **contained;
    t_bool  same_dest_src;
    va_list ap;

    contained = NULL;
    va_start(ap, operation);
    if ((operation == ARRAY_ADDITION && array->contained_size < array->total_size) ||
        (operation == ARRAY_DELETION && array->contained_size % CHUNK_SIZE))
    {
        same_dest_src = TRUE;
        contained = array->contained;
    }
    else
    {
        same_dest_src = FALSE;
        if (!(contained = calloc(new_size + 1, sizeof(void *))))
            return (FALSE);
        array->total_size = new_size;
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
        free(array->contained);
        array->contained = contained;
    }
    va_end(ap);
    return (TRUE);
}

t_bool          array_insert_at(Object *container, void *data, t_type type, ssize_t pos)
{
    Array       *self;
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

t_bool      array_delete_at(Object *container, ssize_t pos)
{
    Array   *self;

    self = container;
    if (pos < 0 || self->contained_size == 0 || pos >= self->contained_size)
        return (FALSE);
    if (array_alloc(self, ARRAY_ALLOC_SIZE(self->contained_size - 1), ARRAY_DELETION, pos) == FALSE)
        return (FALSE);
    return (TRUE);
}

t_bool      array_erase(Object *container)
{
    Array   *self;
    ssize_t i;

    self = container;
    i = -1;
    while (++i < self->contained_size)
    {
        free(((t_data **)self->contained)[i]);
        ((t_data **)self->contained)[i] = NULL;
    }
    self->contained_size = 0;
    return (TRUE);
}

Object          *array_front(const Object *array)
{
    const Array *container;

    container = array;
    return (container->contained && container->contained_size ? ((void **)container->contained)[0] : NULL);
}

Object          *array_back(const Object *array)
{
    const Array *container;

    container = array;
    return (container->contained && container->contained_size ? ((void **)container->contained)[container->contained_size - 1] : NULL);
}

Object          *array_at(const Object *self, ssize_t pos)
{
    const Array *container;

    container = self;
    return (pos >= 0 && pos < container->contained_size && container->contained ? ((void **)container->contained)[pos] : NULL);
}
