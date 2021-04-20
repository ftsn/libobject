#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iterators.h"
#include "lists.h"
#include "dicts.h"
#include "arrays.h"
#include "stringsdef.h"

Object                  *ctor_definition(SplListFwdIterator)
{
    List                *iterated_obj;
    SplListFwdIterator  *it;
    ssize_t             idx_start;
    ssize_t             i;

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
    it->cur = iterated_obj->vtable->front(iterated_obj);
    if (it->cur == NULL)
        return (it);
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    i = 0;
    while (i < idx_start)
    {
        it->vtable->next((Iterator *)it);
        ++i;
    }
    return (it);
}

Object                              *ctor_definition(DblListBidirectionalIterator)
{
    List                            *iterated_obj;
    DblListBidirectionalIterator    *it;
    ssize_t                         idx_start;
    ssize_t                         i;

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
    it->cur = iterated_obj->vtable->front(iterated_obj);
    if (it->cur == NULL)
        return (it);
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    i = 0;
    while (i < idx_start)
    {
        it->vtable->next((Iterator *)it);
        ++i;
    }
    return (it);
}

Object              *ctor_definition(ArrayRaIterator)
{
    Array           *iterated_obj;
    ArrayRaIterator *it;
    ssize_t         idx_start;

    it = args->class;
    if (!it)
        return (NULL);
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    it->vtable->jump((RandomAccessIterator *)it, idx_start);
    if (iterated_obj->contained_size == 0)
    {
        it->reached_the_beginning = 1;
        it->reached_the_end = 1;
    }
    return (it);
}

Object                  *ctor_definition(StringRaIterator)
{
    String              *iterated_obj;
    StringRaIterator    *it;
    ssize_t             idx_start;

    it = args->class;
    if (!it)
        return (NULL);
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    idx_start = args->start_pos;
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    it->vtable->jump((RandomAccessIterator *)it, idx_start);
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
    DictBidirectionalIterator   *it;

    it = args->class;
    if (!it)
        return (NULL);
    iterated_obj = args->iterable;
    it->iterated_obj = iterated_obj;
    if (args->start_pos == END)
    {
        it->internal_idx = iterated_obj->total_size;
        it->vtable->previous((Iterator *)it);
        it->it_idx = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    }
    else
    {
        it->vtable->next((Iterator *)it);
        it->it_idx = 0;
    }
    if (iterated_obj->contained_size == 0)
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

class_definition(ArrayRaIterator, TYPE_ARRAY_ITERATOR, _iterator_dtor)
class_definition(StringRaIterator, TYPE_STRING_ITERATOR, _iterator_dtor)
class_definition(SplListFwdIterator, TYPE_SPL_LIST_ITERATOR, _iterator_dtor)
class_definition(DblListBidirectionalIterator, TYPE_DBL_LIST_ITERATOR, _iterator_dtor)
class_definition(DictBidirectionalIterator, TYPE_DICT_ITERATOR, _iterator_dtor)
