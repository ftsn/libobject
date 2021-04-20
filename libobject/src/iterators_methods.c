#include <stddef.h>
#include "iterators.h"
#include "lists.h"
#include "stringsdef.h"
#include "arrays.h"
#include "dicts.h"

inline t_bool  it_equals(Iterator *it1, Iterator *it2)
{
    return (it1->dereference(it1) == it2->dereference(it2) ? TRUE : FALSE);
}

t_bool      list_it_previous(Iterator *it)
{
    List    *list;

    list = it->iterated_obj;
    if (is_of_type(list, TYPE_CIRCULAR_DOUBLY_LINKED_LIST) && it->it_idx == 0)
    {
        ++it->reached_the_beginning;
        if (list->contained_size > 1)
        {
            it->it_idx = list->contained_size - 1;
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

t_bool      list_it_next(Iterator *it)
{
    List    *list;

    list = it->iterated_obj;
    if (is_clist(list) && it->it_idx + 1 >= list->contained_size)
    {
        ++it->reached_the_end;
        it->it_idx = 0;
        it->cur = ((t_list_data *)it->cur)->next;
        return (TRUE);
    }
    else if (it->it_idx + 1 >= list->contained_size)
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
    return (TRUE);
}

inline Object  *list_it_dereference(Iterator *it)
{
    return (it->cur ? ((t_list_data *)it->cur)->data : NULL);
}

inline Object  *ra_it_dereference(Iterator *it)
{
    return (it->cur);
}

inline t_bool  ra_it_lt(RandomAccessIterator *it1, RandomAccessIterator *it2)
{
    return (it1->it_idx < it2->it_idx ? TRUE : FALSE);
}

inline t_bool  ra_it_gt(RandomAccessIterator *it1, RandomAccessIterator *it2)
{
    return (it1->it_idx > it2->it_idx ? TRUE : FALSE);
}

t_bool                      array_ra_it_next(Iterator *self)
{
    Array                   *array;
    RandomAccessIterator    *it;

    it = (RandomAccessIterator *)self;
    array = it->iterated_obj;
    if (it->ra_idx + 1 >= array->contained_size)
    {
        if (!it->reached_the_end)
            ++it->reached_the_end;
        return (FALSE);
    }
    ++it->it_idx;
    ++it->ra_idx;
    it->cur = ((t_data **)array->contained)[it->ra_idx];
    return (TRUE);
}

t_bool                      array_ra_it_previous(Iterator *self)
{
    Array                   *array;
    RandomAccessIterator    *it;

    it = (RandomAccessIterator *)self;
    array = it->iterated_obj;
    if (it->ra_idx == 0)
    {
        if (!it->reached_the_beginning)
            ++it->reached_the_beginning;
        return (FALSE);
    }
    --it->it_idx;
    --it->ra_idx;
    it->cur = ((t_data **)array->contained)[it->ra_idx];
    return (TRUE);
}

t_bool      array_ra_it_jump(RandomAccessIterator *it, ssize_t idx)
{
    Array   *array;

    array = it->iterated_obj;
    if (it->ra_idx + idx >= array->contained_size || it->ra_idx + idx < 0)
        return (FALSE);
    it->ra_idx += idx;
    it->it_idx += idx;
    it->cur = ((t_data **)array->contained)[it->ra_idx];
    return (TRUE);
}

Object      *array_ra_it_at(RandomAccessIterator *it, ssize_t idx)
{
    Array   *array;

    array = it->iterated_obj;
    return (array->at(array, idx));
}

t_bool                      string_ra_it_next(Iterator *self)
{
    String                  *s;
    RandomAccessIterator    *it;

    it = (RandomAccessIterator *)self;
    s = it->iterated_obj;
    if (it->ra_idx + 1 >= s->contained_size)
    {
        if (!it->reached_the_end)
            ++it->reached_the_end;
        return (FALSE);
    }
    ++it->it_idx;
    ++it->ra_idx;
    it->cur = &((char*)s->contained)[it->ra_idx];
    return (TRUE);
}

t_bool                      string_ra_it_previous(Iterator *self)
{
    String                  *s;
    RandomAccessIterator    *it;

    it = (RandomAccessIterator *)self;
    s = it->iterated_obj;
    if (it->ra_idx == 0)
    {
        if (!it->reached_the_beginning)
            ++it->reached_the_beginning;
        return (FALSE);
    }
    --it->it_idx;
    --it->ra_idx;
    it->cur = &((char *)s->contained)[it->ra_idx];
    return (TRUE);
}

t_bool                      string_ra_it_jump(RandomAccessIterator *self, ssize_t idx)
{
    String                  *s;
    RandomAccessIterator    *it;

    it = (RandomAccessIterator *)self;
    s = it->iterated_obj;
    if (it->ra_idx + idx >= s->contained_size || it->ra_idx + idx < 0)
        return (FALSE);
    it->ra_idx += idx;
    it->it_idx += idx;
    it->cur = &((char *)s->contained)[it->ra_idx];
    return (TRUE);
}

Object      *string_ra_it_at(RandomAccessIterator *it, ssize_t idx)
{
    String  *s;

    s = it->iterated_obj;
    return (s->at(s, idx));
}

t_bool                          dict_bidirectional_it_next(Iterator *it)
{
    DictBidirectionalIterator   *dict_it;
    DblList                     **contained;

    dict_it = (DictBidirectionalIterator *)it;
    contained = ((Dict *)it->iterated_obj)->contained;
    if (it->it_idx == 0 && ((Dict *)it->iterated_obj)->total_size && contained[0])
    {
        ++it->it_idx;
        it->cur = ((DblList *)contained[0])->contained;
        return (TRUE);
    }
    if (it->cur && ((t_list_data *)it->cur)->next)
    {
        ++it->it_idx;
        it->cur = ((t_list_data *)it->cur)->next;
        return (TRUE);
    }
    while (dict_it->internal_idx < ((Dict *)it->iterated_obj)->total_size)
    {
        ++dict_it->internal_idx;
        if (contained[dict_it->internal_idx] != NULL)
            break;
    }
    if (dict_it->internal_idx == ((Dict *)it->iterated_obj)->total_size)
    {
        it->reached_the_end = 1;
        return (FALSE);
    }
    ++it->it_idx;
    it->cur = ((DblList *)contained[dict_it->internal_idx])->contained;
    return (TRUE);
}

t_bool                          dict_bidirectional_it_previous(Iterator *it)
{
    DictBidirectionalIterator   *dict_it;
    DblList                     **contained;

    dict_it = (DictBidirectionalIterator *)it;
    contained = ((Dict *)it->iterated_obj)->contained;
    if (it->cur && ((t_list_data *)it->cur)->prev)
    {
        --it->it_idx;
        it->cur = ((t_list_data *)it->cur)->prev;
        return (TRUE);
    }
    while (dict_it->internal_idx > 0)
    {
        --dict_it->internal_idx;
        if (contained[dict_it->internal_idx] != NULL)
            break;
    }
    if (dict_it->internal_idx == 0 && (contained[dict_it->internal_idx] == NULL || ((DblList *)contained[0])->contained == it->cur))
    {
        it->reached_the_beginning = 1;
        return (FALSE);
    }
    --it->it_idx;
    it->cur = ((DblList *)contained[dict_it->internal_idx])->contained;
    return (TRUE);
}

inline Object  *dict_bidirectional_it_dereference(Iterator *it)
{
    return (it->cur ? ((t_list_data *)it->cur)->data : NULL);
}

Object              *generate_it(Object *self, t_it_type type)
{
    Iterator        *it;

    it = NULL;
    if (is_spl_list(self) == TRUE)
        it = new_obj(SplListFwdIterator, .iterable = self, .start_pos = type);
    else if (is_dbl_list(self) == TRUE)
        it = new_obj(DblListBidirectionalIterator, .iterable = self, .start_pos = type);
    else if (is_of_type(self, TYPE_ARRAY) == TRUE)
        it = new_obj(ArrayRaIterator, .iterable = self, .start_pos = type);
    else if (is_of_type(self, TYPE_DICT) == TRUE)
        it = new_obj(DictBidirectionalIterator, .iterable = self, .start_pos = type);
    else if (is_of_type(self, TYPE_STRING) == TRUE)
        it = new_obj(StringRaIterator, .iterable = self, .start_pos = type);
    return (it);
}