#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "containers.h"
#include "iterators.h"
#include "utils.h"
#include "arrays.h"

Object  *_container_data(const Object *self)
{
    return (((Container *)self)->contained);
}

ssize_t _container_size(const Object *container)
{
    return (((Container *)container)->contained_size);
}

t_bool  _container_empty(const Object *container)
{
    return (((Container *)container)->contained_size == 0 ? TRUE : FALSE);
}

static void copy_array(void **dest, void **src, t_data *typed_data, ssize_t pos)
{
    ssize_t i;
    ssize_t j;

    i = 0;
    if (!dest || !src)
        return;
    while (src[i] && i < pos)
    {
        dest[i] = src[i];
        ++i;
    }
    j = i;
    dest[i++] = typed_data;
    while (src[j])
        dest[i++] = src[j++];
}

t_bool          _container_insert_at(Object *container, void *data, t_type type, ssize_t pos)
{
    void        **res;
    Container   *self;
    t_data      *typed_data;

    self = container;
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (!(res = malloc(sizeof(t_data *) * (self->contained_size + 2))))
    {
        free(typed_data);
        return (FALSE);
    }
    copy_array(res, self->contained, typed_data, pos);
    res[self->contained_size + 1] = NULL;
    free(self->contained);
    self->contained = res;
    ++self->contained_size;
    return (TRUE);
}

t_bool          _container_push_back(Object *self, void *data, t_type type)
{
    Container   *self_c;

    self_c = self;
    return (self_c->insert_at(self_c, data, type, self_c->contained_size));
}

/*
** Basic function provided to the user to allow him to print the container
** without having to create his own function
*/
void typed_basic_print(ssize_t i, const t_data *elem, const char *prefix)
{
    switch (elem->type)
    {
        case TYPE_CHAR:
            printf("%s%d: [%c]\n", prefix, (int)i, *(char *)elem->data);
            break;
        case TYPE_UCHAR:
            printf("%s%d: [%hhu]\n", prefix, (int)i, *(unsigned char *)elem->data);
            break;
        case TYPE_CSTRING:
            printf("%s%d: [%s]\n", prefix, (int)i, (char *)elem->data);
            break;
        case TYPE_INT:
            printf("%s%d: [%d]\n", prefix, (int)i, *(int *)elem->data);
            break;
        case TYPE_UINT:
            printf("%s%d: [%u]\n", prefix, (int)i, *(unsigned int *)elem->data);
            break;
        case TYPE_LONG:
            printf("%s%d: [%ld]\n", prefix, (int)i, *(long *)elem->data);
            break;
        case TYPE_ULONG:
            printf("%s%d: [%lu]\n", prefix, (int)i, *(unsigned long *)elem->data);
            break;
        case TYPE_INT64:
            printf("%s%d: [%" PRId64 "]\n", prefix, (int)i, *(int64_t *)elem->data);
            break;
        case TYPE_UINT64:
            printf("%s%d: [%" PRIu64 "]\n", prefix, (int)i, *(uint64_t *)elem->data);
            break;
        case TYPE_DOUBLE:
            printf("%s%d: [%f]\n", prefix, (int)i, *(double *)elem->data);
            break;
        case TYPE_FLOAT:
            printf("%s%d: [%f]\n", prefix, (int)i, *(float *)elem->data);
            break;
        case TYPE_PTR:
            printf("%s%d: [%p]\n", prefix, (int)i, elem->data);
            break;
        case TYPE_BOOL:
            printf("%s%d: [%s]\n", prefix, (int)i, *(t_bool *)elem->data == TRUE ? "True" : "False");
            break;
        default:;
    }
}

void            _container_print(const Object *container,
                                 const char *title,
                                 void (*f)(ssize_t i, const t_data *elem, const char *prefix),
                                 const char *prefix)
{
    Iterator    *it;
    char        *concat_prefix;
    t_data      *cur;
    char        *recursion_title;
    ssize_t     i;

    if (!(it = ((const Container *)container)->begin(container)))
        return;
    i = 0;
    if (!(concat_prefix = concat(prefix, "  ")))
        return;
    if (title)
        printf("%s%s\n", prefix, title);
    printf("%s[\n", prefix);
    while (!it->reached_the_end)
    {
        cur = it->dereference(it);
        if (is_container(cur))
        {
            if (is_of_type(cur, TYPE_ARRAY) == TRUE)
                recursion_title = "Sub array";
            else if (is_list(cur) == TRUE)
                recursion_title = "Sub list";
            else if (is_of_type(cur, TYPE_DICT) == TRUE)
                recursion_title = "Sub dict";
            else
                recursion_title = "Undefined container";
            printf("%s%d:\n", concat_prefix, (int)i);
            _container_print(cur->data, recursion_title, typed_basic_print, concat_prefix);
        }
        else
            f(i, cur, concat_prefix);
        it->next(it);
        ++i;
    }
    printf("%s]\n", prefix);
    delete(it);
    free(concat_prefix);
}

Object          *_container_to_type(Object *self, Class *type)
{
    Container   *self_c;
    Container   *ctn;
    Iterator    *it;

    self_c = self;
    if (!(ctn = new(type, NULL, 0)))
        return (NULL);
    if (!(it = self_c->begin(self_c)))
    {
        delete(ctn);
        return (NULL);
    }
    while (!it->reached_the_end)
    {
        if (ctn->push_back(ctn, ((t_data *)it->dereference(it))->data, ((t_data *)it->dereference(it))->type) == FALSE)
        {
            delete(ctn);
            delete(it);
            return (NULL);
        }
        it->next(it);
    }
    delete(it);
    return (ctn);
}

Object          *_container_sub(Object *self, Class *type, ssize_t begin, ssize_t len)
{
    Container   *ctn;
    Container   *self_c;
    Iterator    *it;
    ssize_t     i;

    i = 0;
    self_c = (Container *)self;
    if (begin < 0)
        if ((begin = self_c->contained_size + begin) < 0)
            return (NULL);
    if (self_c->contained_size > 0 && begin >= 0 && begin >= self_c->contained_size)
        return (NULL);
    if (begin + len > self_c->contained_size)
        return (NULL);
    if (!(ctn = new(type, NULL, 0)))
        return (NULL);
    if (!(it = self_c->begin(self_c)))
    {
        delete(ctn);
        return (NULL);
    }
    while (i < begin)
    {
        it->next(it);
        ++i;
    }
    i = 0;
    while (i < len)
    {
        if (ctn->push_back(ctn, ((t_data *)it->dereference(it))->data, ((t_data *)it->dereference(it))->type) == FALSE)
        {
            delete (ctn);
            delete (it);
            return (NULL);
        }
        ++i;
        it->next(it);
    }
    delete (it);
    return (ctn);
}

Object          *_container_map(Object *self, Class *type, void *(*fptr)(ssize_t i, void *cur))
{
    Container   *ctn;
    Iterator    *it;
    ssize_t     i;
    t_data      *typed_data;

    if (!(ctn = new (type, NULL, 0)))
        return (NULL);
    if (!(it = ((Container *)self)->begin(self)))
    {
        delete (ctn);
        return (NULL);
    }
    i = 0;
    while (!it->reached_the_end)
    {
        typed_data = fptr(i, it->dereference(it));
        if (ctn->push_back(ctn, typed_data->data, typed_data->type) == FALSE)
        {
            delete (it);
            delete (ctn);
            return (NULL);
        }
        ++i;
        it->next(it);
    }
    delete (it);
    return (ctn);
}

Object  *_container_begin(const Object *self)
{
    return (generate_it(self, BEGIN));
}

Object  *_container_end(const Object *self)
{
    return (generate_it(self, END));
}

t_bool      ctn_copy_ctor(Container *ctn, void **copy, ssize_t size)
{
    ssize_t i;

    i = 0;
    if (size == COPY_ALL)
    {
        size = 0;
        while (copy[size])
            ++size;
    }
    while (i < size)
    {
        if (ctn->push_back(ctn, ((t_data *)copy[i])->data, ((t_data *)copy[i])->type) == FALSE)
            return (FALSE);
        ++i;
    }
    return (TRUE);
}