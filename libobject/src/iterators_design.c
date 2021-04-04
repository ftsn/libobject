#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iterators.h"
#include "lists.h"
#include "dicts.h"

static t_bool   _list_iterator_ctor(Object *self, va_list *args)
{
    Container   *iterated_obj;
    Iterator    *it;
    ssize_t     idx_start;
    ssize_t     i;

    it = self;
    iterated_obj = va_arg(*args, void *);
    it->iterated_obj = iterated_obj;
    it->cur = iterated_obj->front(iterated_obj);
    if (it->cur == NULL)
        return (TRUE);
    idx_start = va_arg(*args, int);
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    i = 0;
    while (i < idx_start)
    {
        it->next(it);
        ++i;
    }
    return (TRUE);
}

static t_bool               _ra_iterator_ctor(Object *self, va_list *args)
{
    Container               *iterated_obj;
    Iterator                *it;
    RandomAccessIterator    *ra_it;
    ssize_t                 idx_start;

    it = self;
    ra_it = (RandomAccessIterator *)it;
    iterated_obj = va_arg(*args, void *);
    it->iterated_obj = iterated_obj;
    idx_start = va_arg(*args, int);
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    ra_it->jump(ra_it, idx_start);
    return (TRUE);
}

static t_bool                   _dict_bidirectional_it_ctor(Object *self, va_list *args)
{
    Dict                        *iterated_obj;
    Iterator                    *it;
    DictBidirectionalIterator   *dict_it;
    ssize_t                     idx_start;

    it = self;
    dict_it = (DictBidirectionalIterator *)it;
    iterated_obj = va_arg(*args, void *);
    it->iterated_obj = iterated_obj;
    idx_start = va_arg(*args, int);
    if (idx_start == END)
    {
        dict_it->internal_idx = iterated_obj->total_size;
        it->previous(it);
    }
    else
        it->next(it);
    return (TRUE);
}

static void _iterator_dtor(Object *self, va_list *args)
{
    (void)self;
    (void)args;
}

static RandomAccessIterator _array_ra_it_descr =
    {
        {
            {
                TYPE_ARRAY_RA_ITERATOR,
                sizeof(RandomAccessIterator),
                &_ra_iterator_ctor,
                &_iterator_dtor
            },
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
    };

Class *_array_ra_it = (Class *)&_array_ra_it_descr;

static RandomAccessIterator _string_ra_it_descr =
    {
        {
            {
                TYPE_STRING_RA_ITERATOR,
                sizeof(RandomAccessIterator),
                &_ra_iterator_ctor,
                &_iterator_dtor
            },
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
    };

Class *_string_ra_it = (Class *)&_string_ra_it_descr;

static ForwardIterator _spl_list_forward_it_descr =
    {
        {
            TYPE_SPL_LIST_FORWARD_ITERATOR,
            sizeof(ForwardIterator),
            &_list_iterator_ctor,
            &_iterator_dtor
        },
        &_it_equals,

        NULL,
        &_list_it_next,

        &_list_it_dereference,

        NULL,
        NULL,
        0,
        0,
        0
    };

Class *_spl_list_forward_it = (Class *)&_spl_list_forward_it_descr;

static BidirectionalIterator _dbl_list_bidirectional_it_descr =
    {
        {
            TYPE_DBL_LIST_BIDIRECTIONAL_ITERATOR,
            sizeof(BidirectionalIterator),
            &_list_iterator_ctor,
            &_iterator_dtor
        },
        &_it_equals,

        &_list_it_previous,
        &_list_it_next,

        &_list_it_dereference,

        NULL,
        NULL,
        0,
        0,
        0
    };

Class *_dbl_list_bidirectional_it = (Class *)&_dbl_list_bidirectional_it_descr;

static DictBidirectionalIterator _dict_ra_it_descr =
    {
        {
            {
                TYPE_DICT_BIDIRECTIONAL_ITERATOR,
                sizeof(DictBidirectionalIterator),
                &_dict_bidirectional_it_ctor,
                &_iterator_dtor,
            },
            &_it_equals,

            &_dict_bidirectional_it_previous,
            &_dict_bidirectional_it_next,

            &_list_it_dereference,

            NULL,
            NULL,
            0,
            0,
            0,
        },
        0,
    };

Class *_dict_ra_it = (Class *)&_dict_ra_it_descr;
