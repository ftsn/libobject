#include <stdlib.h>
#include <string.h>
#include "iterators.h"
#include "lists.h"

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
    idx_start = va_arg(*args, ssize_t);
    if (idx_start == END)
        idx_start = iterated_obj->contained_size - 1;
    i = 0;
    while (i < idx_start)
    {
        it->next(it);
        ++i;
    }
    return (TRUE);
}

static t_bool   _ra_iterator_ctor(Object *self, va_list *args)
{
    Container   *iterated_obj;
    Iterator    *it;
    ssize_t     idx_start;

    it = self;
    iterated_obj = va_arg(*args, void *);
    idx_start = va_arg(*args, ssize_t);
    if (idx_start == END)
        idx_start = iterated_obj->contained_size ? iterated_obj->contained_size - 1 : 0;
    ((RandomAccessIterator *)it)->jump((RandomAccessIterator *)it, idx_start);
    it->iterated_obj = ((RandomAccessIterator *)it)->at((RandomAccessIterator *)it, idx_start);
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

            &_ra_it_previous,
            &_ra_it_next,

            &_ra_it_dereference,

            NULL,
            NULL,
            0,
        },
        &_ra_it_lt,
        &_ra_it_gt,

        &_ra_it_jump,

        &_ra_it_at,

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

            &_ra_it_previous,
            &_ra_it_next,

            &_ra_it_dereference,

            NULL,
            NULL,
            0,
        },
        &_ra_it_lt,
        &_ra_it_gt,

        &_ra_it_jump,

        &_ra_it_at,

        0
    };

Class *_string_ra_it = (Class *)&_string_ra_it_descr;

static RandomAccessIterator _dict_ra_it_descr =
    {
        {
            {
                TYPE_DICT_RA_ITERATOR,
                sizeof(RandomAccessIterator),
                &_ra_iterator_ctor,
                &_iterator_dtor
            },
            &_it_equals,

            &_ra_it_previous,
            &_ra_it_next,

            &_ra_it_dereference,

            NULL,
            NULL,
            0,
        },
        &_ra_it_lt,
        &_ra_it_gt,

        &_ra_it_jump,

        &_ra_it_at,

        0
    };

Class *_dict_ra_it = (Class *)&_dict_ra_it_descr;

static ForwardIterator _spl_list_forward_it_descr =
    {
        {
            TYPE_SPL_LIST_FORWARD_ITERATOR,
            sizeof(ForwardIterator),
            &_list_iterator_ctor,
            &_iterator_dtor
        },
        &_it_equals,

        &_list_it_next,

        &_list_it_dereference,

        NULL,
        NULL,
        0,
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
    };

Class *_dbl_list_bidirectional_it = (Class *)&_dbl_list_bidirectional_it_descr;
