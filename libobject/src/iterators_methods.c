#include <stddef.h>
#include "iterators.h"
#include "lists.h"

inline t_bool  _it_equals(Iterator *it1, Iterator *it2)
{
    return (it1->dereference(it1) == it2->dereference(it2) ? TRUE : FALSE);
}

t_bool  _list_it_previous(Iterator *it)
{
    if(((t_list_data *)it->cur)->prev == NULL)
        return (FALSE);
    --it->it_idx;
    it->cur = ((t_list_data *)it->cur)->prev;
    return (TRUE);
}

t_bool  _list_it_next(Iterator *it)
{
    if(((t_list_data *)it->cur)->next == NULL)
        return (FALSE);
    ++it->it_idx;
    it->cur = ((t_list_data *)it->cur)->next;
    return (TRUE);
}

inline t_bool   _ra_it_previous(Iterator *it)
{
    return (_ra_it_jump((RandomAccessIterator *)it, -1));
}

inline t_bool   _ra_it_next(Iterator *it)
{
    return (_ra_it_jump((RandomAccessIterator *)it, 1));
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

t_bool          _ra_it_jump(RandomAccessIterator *it, ssize_t idx)
{
    Container   *ctn;

    ctn = ((Iterator *)it)->iterated_obj;
    if (it->ra_idx + idx >= ctn->contained_size)
        return (FALSE);
    it->ra_idx += idx;
    ((Iterator *)it)->it_idx += idx;
    return (TRUE);
}

Object          *_ra_it_at(RandomAccessIterator *it, ssize_t idx)
{
    Container   *ctn;

    ctn = ((Iterator *)it)->iterated_obj;
    return (ctn->at(ctn, idx));
}