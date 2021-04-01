#include <stddef.h>
#include <stdio.h>
#include "iterators.h"
#include "lists.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline t_bool  _it_equals(Iterator *it1, Iterator *it2)
{
    return (it1->dereference(it1) == it2->dereference(it2) ? TRUE : FALSE);
}

t_bool  _list_it_previous(Iterator *it)
{
    if(it->it_idx == 0)
        return (FALSE);
    --it->it_idx;
    it->cur = ((t_list_data *)it->cur)->prev;
    return (TRUE);
}

t_bool      _list_it_next(Iterator *it)
{
    if(it->it_idx + 1 >= ((Container *)it->iterated_obj)->contained_size)
        return (FALSE);
    ++it->it_idx;
    it->cur = ((t_list_data *)it->cur)->next;
    return (TRUE);
}

inline t_bool   _ra_it_previous(Iterator *it)
{
    return ((RandomAccessIterator *)it)->jump((RandomAccessIterator *)it, -1);
}

inline t_bool   _ra_it_next(Iterator *it)
{
    return ((RandomAccessIterator *)it)->jump((RandomAccessIterator *)it, 1);
}


Object  *_list_it_dereference(Iterator *it)
{
    return (((t_list_data *)it->cur)->data);
}

inline Object  *_ra_it_dereference(Iterator *it)
{
    return (it->cur);
}

inline t_bool  _ra_it_lt(RandomAccessIterator *it1, RandomAccessIterator *it2)
{
    return (it1 < it2 ? TRUE : FALSE);
}

inline t_bool  _ra_it_gt(RandomAccessIterator *it1, RandomAccessIterator *it2)
{
    return (it1 > it2 ? TRUE : FALSE);
}

t_bool          _array_ra_it_jump(RandomAccessIterator *it, ssize_t idx)
{
    Container   *ctn;

    ctn = ((Iterator *)it)->iterated_obj;
    if (it->ra_idx + idx >= ctn->contained_size)
        return (FALSE);
    it->ra_idx += idx;
    ((Iterator *)it)->it_idx += idx;
    ((Iterator *)it)->cur = ((t_data **)ctn->contained)[it->ra_idx];
    return (TRUE);
}

Object          *_ra_it_at(RandomAccessIterator *it, ssize_t idx)
{
    Container   *ctn;

    ctn = ((Iterator *)it)->iterated_obj;
    return (ctn->at(ctn, idx));
}

Object              *generate_it(const Object *self, t_it_type type)
{
    Iterator        *it;
    const Container *ctn;

    ctn = self;
    it = NULL;
    if (is_spl_list(ctn) == TRUE)
        it = new(_spl_list_forward_it, self, type);
    else if (is_dbl_list(ctn) == TRUE)
        it = new(_dbl_list_bidirectional_it, self, type);
    else if (is_of_type(ctn, TYPE_ARRAY) == TRUE)
        it = new(_array_ra_it, self, type);
    else if (is_of_type(ctn, TYPE_DICT) == TRUE)
        it = new(_dict_ra_it, self, type);
    else if (is_of_type(ctn, TYPE_STRING) == TRUE)
        it = new(_string_ra_it, self, type);
    return (it);
}