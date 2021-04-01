#include <stddef.h>
#include "iterators.h"
#include "lists.h"

inline t_bool  _it_equals(Iterator *it1, Iterator *it2)
{
    return (it1->dereference(it1) == it2->dereference(it2) ? TRUE : FALSE);
}

t_bool          _list_it_previous(Iterator *it)
{
    Container   *ctn;

    ctn = it->iterated_obj;
    if (is_of_type(ctn, TYPE_CIRCULAR_DOUBLY_LINKED_LIST) && it->it_idx == 0)
    {
        ++it->reached_the_beginning;
        if (ctn->contained_size > 1)
        {
            --it->it_idx;
            it->cur = ((t_list_data *)it->cur)->prev;
        }
        return (TRUE);
    }
    else if (it->it_idx == 0)
    {
        if (!it->reached_the_beginning)
            ++it->reached_the_beginning;
        return (FALSE);
    }
    else
    {
        --it->it_idx;
        it->cur = ((t_list_data *)it->cur)->prev;
    }
    return (TRUE);
}

t_bool          _list_it_next(Iterator *it)
{
    Container   *ctn;

    ctn = it->iterated_obj;
    if (is_clist(ctn) && it->it_idx + 1 >= ctn->contained_size)
    {
        ++it->reached_the_end;
        it->it_idx = 0;
        it->cur = ((t_list_data *)it->cur)->next;
        return (TRUE);
    }
    else if (it->it_idx + 1 >= ctn->contained_size)
    {
        if (!it->reached_the_end)
            ++it->reached_the_end;
        return (FALSE);
    }
    else
    {
        ++it->it_idx;
        it->cur = ((t_list_data *)it->cur)->next;
    }


    if (it->it_idx + 1 >= ((Container *)it->iterated_obj)->contained_size)
        return (FALSE);
    ++it->it_idx;
    it->cur = ((t_list_data *)it->cur)->next;
    return (TRUE);
}

inline t_bool   _ra_it_next(Iterator *it)
{
    Container   *ctn;

    ctn = it->iterated_obj;
    if (((RandomAccessIterator *)it)->ra_idx + 1 >= ctn->contained_size)
    {
        if (!it->reached_the_end)
            ++it->reached_the_end;
        return (FALSE);
    }
    ++it->it_idx;
    ++((RandomAccessIterator *)it)->ra_idx;
    ((Iterator *)it)->cur = ((t_data **)ctn->contained)[((RandomAccessIterator *)it)->ra_idx];
    return (TRUE);
}

inline t_bool   _ra_it_previous(Iterator *it)
{
    Container   *ctn;

    ctn = it->iterated_obj;
    if (((RandomAccessIterator *)it)->ra_idx == 0)
    {
        if (!it->reached_the_beginning)
            ++it->reached_the_beginning;
        return (FALSE);
    }
    --it->it_idx;
    --((RandomAccessIterator *)it)->ra_idx;
    ((Iterator *)it)->cur = ((t_data **)ctn->contained)[((RandomAccessIterator *)it)->ra_idx];
    return (TRUE);
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
    if (it->ra_idx + idx >= ctn->contained_size || it->ra_idx + idx < 0)
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