#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iterators.h"
#include "lists.h"
#include "dicts.h"
#include "stringsdef.h"

Object          *ctor_definition(SplListFwdIterator)
{
    Container   *iterated_obj;
    Iterator    *it;
    ssize_t     idx_start;
    ssize_t     i;

    it = args->class;
    if (!it)
        return (NULL);
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    if (iterated_obj->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    it->cur = iterated_obj->front(iterated_obj);
    if (it->cur == NULL)
        return (it);
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    i = 0;
    while (i < idx_start)
    {
        it->next(it);
        ++i;
    }
    return (it);
}

Object          *ctor_definition(DblListBidirectionalIterator)
{
    Container   *iterated_obj;
    Iterator    *it;
    ssize_t     idx_start;
    ssize_t     i;

    it = args->class;
    if (!it)
        return (NULL);
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    if (iterated_obj->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    it->cur = iterated_obj->front(iterated_obj);
    if (it->cur == NULL)
        return (it);
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    i = 0;
    while (i < idx_start)
    {
        it->next(it);
        ++i;
    }
    return (it);
}

Object                      *ctor_definition(ArrayRaIterator)
{
    Container               *iterated_obj;
    Iterator                *it;
    RandomAccessIterator    *ra_it;
    ssize_t                 idx_start;

    it = args->class;
    if (!it)
        return (NULL);
    ra_it = (RandomAccessIterator *)it;
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    ra_it->jump(ra_it, idx_start);
    if (iterated_obj->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    return (it);
}

Object                      *ctor_definition(StringRaIterator)
{
    String                  *iterated_obj;
    Iterator                *it;
    RandomAccessIterator    *ra_it;
    ssize_t                 idx_start;

    it = args->class;
    if (!it)
        return (NULL);
    ra_it = (RandomAccessIterator *)it;
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    ra_it->jump(ra_it, idx_start);
    if (iterated_obj->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    return (it);
}

Object                          *ctor_definition(DictBidirectionalIterator)
{
    Dict                        *iterated_obj;
    Iterator                    *it;
    DictBidirectionalIterator   *dict_it;

    it = args->class;
    if (!it)
        return (NULL);
    dict_it = (DictBidirectionalIterator *)it;
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    if (args->start_pos == END)
    {
        dict_it->internal_idx = iterated_obj->total_size;
        it->previous(it);
        it->it_idx = ((Container *)iterated_obj)->contained_size ? ((Container *)iterated_obj)->contained_size - 1 : 0;
    }
    else
    {
        it->next(it);
        it->it_idx = 0;
    }
    if (((Container *)iterated_obj)->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    return (it);
}

static void _iterator_dtor(Object *self)
{
    (void)self;
}

class_definition(ArrayRaIterator,
    {
        class_metadata(ArrayRaIterator, TYPE_ARRAY_ITERATOR, _iterator_dtor),
        &_it_equals,

        &_array_ra_it_previous,
        &_array_ra_it_next,

        &_ra_it_dereference,

        NULL,
        NULL,
        0,
        0,
        0
    },
    &_ra_it_lt,
    &_ra_it_gt,

    &_array_ra_it_jump,

    &_array_ra_it_at,

    0
)

class_definition(StringRaIterator,
    {
        class_metadata(StringRaIterator, TYPE_STRING_ITERATOR, _iterator_dtor),
        &_it_equals,

        &_string_ra_it_previous,
        &_string_ra_it_next,

        &_ra_it_dereference,

        NULL,
        NULL,
        0,
        0,
        0
    },
    &_ra_it_lt,
    &_ra_it_gt,

    &_string_ra_it_jump,

    &_string_ra_it_at,

    0
)

class_definition(SplListFwdIterator,
    class_metadata(SplListFwdIterator, TYPE_SPL_LIST_ITERATOR, _iterator_dtor),
    &_it_equals,

    NULL,
    &_list_it_next,

    &_list_it_dereference,

    NULL,
    NULL,
    0,
    0,
    0
)

class_definition(DblListBidirectionalIterator,
    class_metadata(DblListBidirectionalIterator, TYPE_DBL_LIST_ITERATOR, _iterator_dtor),
    &_it_equals,

    &_list_it_previous,
    &_list_it_next,

    &_list_it_dereference,

    NULL,
    NULL,
    0,
    0,
    0,
)

class_definition(DictBidirectionalIterator,
    {
        class_metadata(DictBidirectionalIterator, TYPE_DICT_ITERATOR, _iterator_dtor),
        &_it_equals,

        &_dict_bidirectional_it_previous,
        &_dict_bidirectional_it_next,

        &_dict_bidirectional_it_dereference,

        NULL,
        NULL,
        0,
        0,
        0,
    },
    0,
)
