#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h>
#include "containers.h"
#include "iterators.h"
#include "utils.h"
#include "arrays.h"
#include "dicts.h"

Object  *container_data(const Object *self)
{
    return (((Container *)self)->contained);
}

ssize_t container_size(const Object *container)
{
    return (((Container *)container)->contained_size);
}

t_bool  container_empty(const Object *container)
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

t_bool          container_insert_at(Object *container, void *data, t_type type, ssize_t pos)
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

t_bool          container_push_back(Object *self, void *data, t_type type)
{
    Container   *self_c;

    self_c = self;
    return (self_c->insert_at(self_c, data, type, self_c->contained_size));
}

/*
** Basic function provided to the user to allow him to print the container
** without having to create his own function
*/
void typed_basic_print(const t_data *elem, const char *prefix)
{
    switch (elem->type)
    {
        case TYPE_CHAR:
            printf("[%c]\n", *(char *)elem->data);
            break;
        case TYPE_UCHAR:
            printf("[%hhu]\n", *(unsigned char *)elem->data);
            break;
        case TYPE_CSTRING:
            printf("[%s]\n", (char *)elem->data);
            break;
        case TYPE_SHORT:
            printf("[%hd]\n", *(short *)elem->data);
            break;
        case TYPE_USHORT:
            printf("[%hu]\n", *(unsigned short *)elem->data);
            break;
        case TYPE_INT:
            printf("[%d]\n", *(int *)elem->data);
            break;
        case TYPE_UINT:
            printf("[%u]\n", *(unsigned int *)elem->data);
            break;
        case TYPE_LONG:
            printf("[%ld]\n", *(long *)elem->data);
            break;
        case TYPE_ULONG:
            printf("[%lu]\n", *(unsigned long *)elem->data);
            break;
        case TYPE_INT32:
            printf("[%" PRId32 "]\n", *(int32_t *)elem->data);
            break;
        case TYPE_UINT32:
            printf("[%" PRIu32 "]\n", *(uint32_t *)elem->data);
            break;
        case TYPE_INT64:
            printf("[%" PRId64 "]\n", *(int64_t *)elem->data);
            break;
        case TYPE_UINT64:
            printf("[%" PRIu64 "]\n", *(uint64_t *)elem->data);
            break;
        case TYPE_LONGLONG:
            printf("[%lld]\n", *(long long *)elem->data);
            break;
        case TYPE_ULONGLONG:
            printf("[%llu]\n", *(unsigned long long *)elem->data);
            break;
        case TYPE_INTMAX:
            printf("[%jd]\n", *(intmax_t *)elem->data);
            break;
        case TYPE_UINTMAX:
            printf("[%ju]\n", *(uintmax_t *)elem->data);
            break;
        case TYPE_DOUBLE:
            printf("[%f]\n", *(double *)elem->data);
            break;
        case TYPE_FLOAT:
            printf("[%f]\n", *(float *)elem->data);
            break;
        case TYPE_PTR:
            printf("[%p]\n", elem->data);
            break;
        case TYPE_PTRDIFF:
            printf("[%tx]\n", *(ptrdiff_t *)elem->data);
            break;
        case TYPE_BOOL:
            printf("[%s]\n", *(t_bool *)elem->data == TRUE ? "True" : "False");
            break;
        case TYPE_PAIR:
            printf("[%s] => ", ((const t_pair *)elem->data)->key);
            if (is_container(elem->data) == TRUE)
                container_print(elem->data, "Sub container", typed_basic_print, "");
            else
                typed_basic_print(elem->data, prefix);
            break;
        default:
            printf("Type not detected: [%p]\n", elem->data);
    }
}

void            container_print(Object *container, const char *title, void (*f)(const t_data *elem, const char *prefix), const char *prefix)
{
    Iterator    *it;
    char        *concat_prefix;
    t_data      *cur;
    char        *recursion_title;
    ssize_t     i;

    if (!(it = ((Container *)container)->begin(container)))
        return;
    i = 0;
    if (!(concat_prefix = concat(prefix, "  ")))
        return;
    if (title)
        printf("%s\n", title);
    printf("%s[\n", prefix);
    if (is_of_type(container, TYPE_DICT) == TRUE)
        printf("%sContained size: %zd / In memory size: %zd\n", concat_prefix, ((const Container *)container)->contained_size, ((const Dict *)container)->total_size);
    while (!it->reached_the_end)
    {
        cur = it->dereference(it);
        if (is_container(cur) == TRUE)
        {
            if (is_of_type(cur, TYPE_ARRAY) == TRUE)
                recursion_title = "Sub array";
            else if (is_list(cur) == TRUE)
                recursion_title = "Sub list";
            else if (is_of_type(cur, TYPE_DICT) == TRUE)
                recursion_title = "Sub dict";
            else
                recursion_title = "Undefined container";
            printf("%s%zd: ", concat_prefix, i);
            container_print(cur->data, recursion_title, typed_basic_print, concat_prefix);
        }
        else
        {
            printf("%s%zd: ", concat_prefix, i);
            f(cur, concat_prefix);
        }
        it->next(it);
        ++i;
    }
    printf("%s]\n", prefix);
    delete(it);
    free(concat_prefix);
}

Object          *container_to_type(Object *self, Class *type)
{
    Container   *self_c;
    Container   *ctn;
    Iterator    *it;

    self_c = self;
    if (!(ctn = shallow_new_obj(type)))
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

Object          *container_sub(Object *self, Class *type, ssize_t begin, ssize_t len)
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
    if (!(ctn = shallow_new_obj(type)))
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

Object          *container_map(Object *self, Class *type, t_data (*fptr)(ssize_t i, void *cur))
{
    Container   *ctn;
    Iterator    *it;
    ssize_t     i;
    t_data      typed_data;

    if (!(ctn = shallow_new_obj(type)))
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
        if (ctn->push_back(ctn, typed_data.data, typed_data.type) == FALSE)
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

Object  *container_begin(Object *self)
{
    return (generate_it(self, BEGIN));
}

Object  *container_end(Object *self)
{
    return (generate_it(self, END));
}

t_bool      ctn_copy_ctor(Container *ctn, t_data **copy, ssize_t size)
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