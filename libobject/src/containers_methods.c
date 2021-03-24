#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "containers.h"
#include "iterators.h"
#include "utils.h"

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

t_bool          _container_push_back(Object *self, void *data)
{
    Container   *self_c;

    self_c = self;
    return (self_c->insert_at(self_c, data, self_c->contained_size));
}

/*
** Basic function provided to the user to allow him to print the container
** without having to create his own function
*/
static void _array_basic_print(ssize_t i, const t_data *elem, const char *prefix)
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
    ssize_t     i;

    if (!(it = ((const Container *)container)->first(container)))
        return;
    i = 0;
    if (!(concat_prefix = concat(prefix, "  ")))
        return;
    if (title)
        printf("%s%s\n", prefix, title);
    printf("%s[\n", prefix);
    while ((cur = it->rvalue(it)) != NULL)
    {
        if (is_of_type(cur, TYPE_ARRAY))
        {
            printf("%s%d:\n", concat_prefix, (int)i);
            _container_print(cur->data, "Sub array", _array_basic_print, concat_prefix);
        }
        else
            f(i, cur, concat_prefix);
        ++i;
        it->incr(it);
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
    if (!(ctn = new (type, NULL, 0)))
        return (NULL);
    if (!(it = self_c->first(self_c)))
    {
        delete (ctn);
        return (NULL);
    }
    while (it->rvalue(it) != NULL)
    {
        if (ctn->push_back(ctn, it->rvalue(it)) == FALSE)
        {
            delete (ctn);
            delete (it);
            return (NULL);
        }
        it->incr(it);
    }
    delete (it);
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
        begin = self_c->contained_size - (-begin);
    if (self_c->contained_size > 0 && begin >= 0 && begin >= self_c->contained_size)
        return (NULL);
    if (!(ctn = new (type, NULL, 0)))
        return (NULL);
    if (!(it = self_c->first(self_c)))
    {
        delete (ctn);
        return (NULL);
    }
    it->jump(it, begin);
    while (i < len && begin + i < self_c->contained_size)
    {
        if (ctn->push_back(ctn, it->rvalue(it)) == FALSE)
        {
            delete (ctn);
            delete (it);
            return (NULL);
        }
        ++i;
        it->incr(it);
    }
    delete (it);
    return (ctn);
}

Object          *_container_map(Object *self, Class *type, void *(*fptr)(ssize_t i, void *cur))
{
    Container   *ctn;
    Iterator    *it;
    ssize_t     i;

    if (!(ctn = new (type, NULL, 0)))
        return (NULL);
    if (!(it = ((Container *)self)->first(self)))
    {
        delete (ctn);
        return (NULL);
    }
    i = 0;
    while (it->rvalue(it) != NULL)
    {
        if (ctn->push_back(ctn, fptr(i, it->rvalue(it))) == FALSE)
        {
            delete (it);
            delete (ctn);
            return (NULL);
        }
        ++i;
        it->incr(it);
    }
    delete (it);
    return (ctn);
}

static Object       *generate_it(const Object *self, t_it_type type)
{
    Iterator        *it;
    const Container *ctn;

    ctn = self;
    it = NULL;
    if (ctn->base.__type__ == TYPE_LINKED_LIST ||
        ctn->base.__type__ == TYPE_CIRCULAR_LINKED_LIST ||
        ctn->base.__type__ == TYPE_DOUBLY_LINKED_LIST ||
        ctn->base.__type__ == TYPE_CIRCULAR_DOUBLY_LINKED_LIST)
        it = new (_list_it, self, type);
    if (ctn->base.__type__ == TYPE_ARRAY)
        it = new (_array_it, self, type);
    if (ctn->base.__type__ == TYPE_DICT)
        it = new (_dict_it, self, type);
    if (ctn->base.__type__ == TYPE_STRING)
        it = new (_string_it, self, type);
    return (it);
}

Object  *_container_begin(const Object *self)
{
    return (generate_it(self, BASIC));
}

Object  *_container_last(const Object *self)
{
    return (generate_it(self, REVERSE));
}
