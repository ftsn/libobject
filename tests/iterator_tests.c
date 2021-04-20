#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "stringsdef.h"
#include "arrays.h"
#include "lists.h"
#include "dicts.h"
#include "iterators.h"

static int setup_foo_bar_typed_array(void **state)
{
    t_data  **data_tab;

    data_tab = cstrings_to_data_array((char *[]){"foo", "bar", NULL});
    *state = data_tab;
    return (0);
}

static int teardown_foo_bar_array(void **state)
{
    free_typed_array((t_data **)*state);
    *state = NULL;
    return (0);
}

static void     iterator_empty_array_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Array);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_empty_array_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Array);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_array_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Array, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    assert_int_equal(it->vtable->next(it), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_array_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Array, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    assert_int_equal(it->vtable->previous(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->previous(it), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_array_arithmetic(void **state)
{
    Container               *ctn;
    Iterator                *it;
    RandomAccessIterator    *ra;

    ctn = new_obj(Array, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_int_equal(ctn->vtable->push_back(ctn, "baz", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->vtable->push_back(ctn, "barz", TYPE_CSTRING), TRUE);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 4);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->vtable->begin(ctn);
    ra = (RandomAccessIterator *)it;
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(ra->vtable->jump(ra, 2), TRUE);
    assert_int_equal(ra->vtable->jump(ra, 2), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 2);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "baz");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 3);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "barz");
    assert_int_equal(ra->vtable->jump(ra, -3), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_array_compare_its(void **state)
{
    Container               *ctn;
    Iterator                *it1, *it2;
    RandomAccessIterator    *ra1, *ra2;

    ctn = new_obj(Array, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it1 = ctn->vtable->begin(ctn);
    assert_non_null(it1);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 0);
    assert_string_equal(((t_data *)it1->vtable->dereference(it1))->data, "foo");
    it2 = ctn->vtable->begin(ctn);
    assert_non_null(it2);
    assert_int_equal(it2->reached_the_beginning, 0);
    assert_int_equal(it2->reached_the_end, 0);
    assert_int_equal(it2->it_idx, 0);
    assert_string_equal(((t_data *)it2->vtable->dereference(it2))->data, "foo");
    assert_int_equal(it1->vtable->equals(it1, it2), TRUE);
    assert_int_equal(it1->vtable->next(it1), TRUE);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 1);
    ra1 = (RandomAccessIterator *)it1;
    ra2 = (RandomAccessIterator *)it2;
    assert_int_equal(ra1->vtable->lt(ra1, ra2), FALSE);
    assert_int_equal(ra1->vtable->gt(ra1, ra2), TRUE);
    assert_int_equal(ra2->vtable->lt(ra2, ra1), TRUE);
    assert_int_equal(ra2->vtable->gt(ra2, ra1), FALSE);
    delete(it1);
    delete(it2);
    delete(ctn);
    (void)state;
}

static void     iterator_null_string_begin(void **state)
{
    String      *str;
    Iterator    *it;

    str = new_obj(String);
    assert_non_null(str);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    it = str->vtable->begin(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(str);
    (void)state;
}

static void     iterator_null_string_end(void **state)
{
    String      *str;
    Iterator    *it;

    str = new_obj(String);
    assert_non_null(str);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    it = str->vtable->end(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(str);
    (void)state;
}

static void     iterator_empty_string_begin(void **state)
{
    String      *str;
    Iterator    *it;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    it = str->vtable->begin(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(str);
    (void)state;
}

static void     iterator_empty_string_end(void **state)
{
    String      *str;
    Iterator    *it;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    it = str->vtable->end(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(str);
    (void)state;
}

static void     iterator_non_empty_string_begin(void **state)
{
    String      *str;
    Iterator    *it;
    char        foobar[] = "foobar";

    str = new_obj(String, .to_copy = foobar, .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 6);
    it = str->vtable->begin(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    while (!it->reached_the_end)
    {
        assert_string_equal(it->vtable->dereference(it), foobar + it->it_idx);
        it->vtable->next(it);
    }
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 5);
    delete(it);
    delete(str);
    (void)state;
}

static void     iterator_non_empty_string_end(void **state)
{
    String      *str;
    Iterator    *it;
    char        foobar[] = "foobar";

    str = new_obj(String, .to_copy = foobar, .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 6);
    it = str->vtable->end(str);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 5);
    while (!it->reached_the_beginning)
    {
        assert_string_equal(it->vtable->dereference(it), foobar + ((RandomAccessIterator *)it)->ra_idx);
        it->vtable->previous(it);
    }
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    delete(it);
    delete(str);
    (void)state;
}

static void                 iterator_non_empty_string_arithmetic(void **state)
{
    String                  *str;
    Iterator                *it;
    RandomAccessIterator    *ra;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 6);
    it = str->vtable->begin(str);
    ra = (RandomAccessIterator *)it;
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(it->vtable->dereference(it), "foobar");
    assert_int_equal(ra->vtable->jump(ra, 3), TRUE);
    assert_int_equal(ra->vtable->jump(ra, 3), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 3);
    assert_string_equal(it->vtable->dereference(it), "bar");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 4);
    assert_string_equal(it->vtable->dereference(it), "ar");
    assert_int_equal(ra->vtable->jump(ra, -4), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(it->vtable->dereference(it), "foobar");
    delete(it);
    delete(str);
    (void)state;
}

static void                 iterator_non_empty_string_compare_its(void **state)
{
    String                  *str;
    Iterator                *it1, *it2;
    RandomAccessIterator    *ra1, *ra2;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_int_equal(str->contained_size, 6);
    it1 = str->vtable->begin(str);
    assert_non_null(it1);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 0);
    assert_string_equal(it1->vtable->dereference(it1), "foobar");
    it2 = str->vtable->begin(str);
    assert_non_null(it2);
    assert_int_equal(it2->reached_the_beginning, 0);
    assert_int_equal(it2->reached_the_end, 0);
    assert_int_equal(it2->it_idx, 0);
    assert_string_equal(it2->vtable->dereference(it2), "foobar");
    assert_int_equal(it1->vtable->equals(it1, it2), TRUE);
    assert_int_equal(it1->vtable->next(it1), TRUE);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 1);
    ra1 = (RandomAccessIterator *)it1;
    ra2 = (RandomAccessIterator *)it2;
    assert_int_equal(ra1->vtable->lt(ra1, ra2), FALSE);
    assert_int_equal(ra1->vtable->gt(ra1, ra2), TRUE);
    assert_int_equal(ra2->vtable->lt(ra2, ra1), TRUE);
    assert_int_equal(ra2->vtable->gt(ra2, ra1), FALSE);
    delete(it1);
    delete(it2);
    delete(str);
    (void)state;
}

static void     iterator_empty_spl_list_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_spl_list_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_spl_list_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->vtable->next(it), FALSE);
    else
        assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->it_idx, 1);
    else
        assert_int_equal(it->it_idx, 0);
    if (is_clist(ctn) == FALSE)
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    else
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_spl_list_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->end(ctn);
    if (is_dbl_list(ctn) == TRUE)
    {
        assert_non_null(it);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 1);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
        assert_int_equal(it->vtable->previous(it), TRUE);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 0);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->vtable->previous(it), FALSE);
        else
            assert_int_equal(it->vtable->previous(it), TRUE);
        assert_non_null(it->vtable->dereference(it));
        assert_int_equal(it->reached_the_beginning, 1);
        assert_int_equal(it->reached_the_end, 0);
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->it_idx, 0);
        else
            assert_int_equal(it->it_idx, 1);
        if (is_clist(ctn) == FALSE)
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        else
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    }
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_spl_clist_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplClist);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_spl_clist_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplClist);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_spl_clist_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplClist, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->vtable->next(it), FALSE);
    else
        assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->it_idx, 1);
    else
        assert_int_equal(it->it_idx, 0);
    if (is_clist(ctn) == FALSE)
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    else
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_spl_clist_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(SplClist, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->end(ctn);
    if (is_dbl_list(ctn) == TRUE)
    {
        assert_non_null(it);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 1);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
        assert_int_equal(it->vtable->previous(it), TRUE);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 0);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->vtable->previous(it), FALSE);
        else
            assert_int_equal(it->vtable->previous(it), TRUE);
        assert_non_null(it->vtable->dereference(it));
        assert_int_equal(it->reached_the_beginning, 1);
        assert_int_equal(it->reached_the_end, 0);
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->it_idx, 0);
        else
            assert_int_equal(it->it_idx, 1);
        if (is_clist(ctn) == FALSE)
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        else
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    }
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_dbl_list_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_dbl_list_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_dbl_list_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->vtable->next(it), FALSE);
    else
        assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->it_idx, 1);
    else
        assert_int_equal(it->it_idx, 0);
    if (is_clist(ctn) == FALSE)
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    else
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_dbl_list_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->end(ctn);
    if (is_dbl_list(ctn) == TRUE)
    {
        assert_non_null(it);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 1);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
        assert_int_equal(it->vtable->previous(it), TRUE);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 0);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->vtable->previous(it), FALSE);
        else
            assert_int_equal(it->vtable->previous(it), TRUE);
        assert_non_null(it->vtable->dereference(it));
        assert_int_equal(it->reached_the_beginning, 1);
        assert_int_equal(it->reached_the_end, 0);
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->it_idx, 0);
        else
            assert_int_equal(it->it_idx, 1);
        if (is_clist(ctn) == FALSE)
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        else
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    }
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_dbl_clist_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblClist);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_dbl_clist_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblClist);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_dbl_clist_begin(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblClist, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->vtable->next(it), FALSE);
    else
        assert_int_equal(it->vtable->next(it), TRUE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    if (is_clist(ctn) == FALSE)
        assert_int_equal(it->it_idx, 1);
    else
        assert_int_equal(it->it_idx, 0);
    if (is_clist(ctn) == FALSE)
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    else
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_non_empty_dbl_clist_end(void **state)
{
    Container *ctn;
    Iterator *it;

    ctn = new_obj(DblClist, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    it = ctn->vtable->end(ctn);
    if (is_dbl_list(ctn) == TRUE)
    {
        assert_non_null(it);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 1);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
        assert_int_equal(it->vtable->previous(it), TRUE);
        assert_int_equal(it->reached_the_beginning, 0);
        assert_int_equal(it->reached_the_end, 0);
        assert_int_equal(it->it_idx, 0);
        assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->vtable->previous(it), FALSE);
        else
            assert_int_equal(it->vtable->previous(it), TRUE);
        assert_non_null(it->vtable->dereference(it));
        assert_int_equal(it->reached_the_beginning, 1);
        assert_int_equal(it->reached_the_end, 0);
        if (is_clist(ctn) == FALSE)
            assert_int_equal(it->it_idx, 0);
        else
            assert_int_equal(it->it_idx, 1);
        if (is_clist(ctn) == FALSE)
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "foo");
        else
            assert_string_equal(((t_data *)it->vtable->dereference(it))->data, "bar");
    }
    delete (it);
    delete (ctn);
    (void)state;
}

static void     iterator_empty_dict_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_empty_dict_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_null(it->vtable->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_dict_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;
    t_pair      *pair;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(((Dict *)ctn)->vtable->push(ctn, (unsigned char *)"baz", "barz", TYPE_CSTRING), TRUE);
    assert_int_equal(((Dict *)ctn)->vtable->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    it = ctn->vtable->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "barz");
    assert_int_equal(it->vtable->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "bar");
    assert_int_equal(it->vtable->next(it), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "bar");
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_dict_end(void **state)
{
    Container   *ctn;
    Iterator    *it;
    t_pair      *pair;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(((Dict *)ctn)->vtable->push(ctn, (unsigned char *)"baz", "barz", TYPE_CSTRING), TRUE);
    assert_int_equal(((Dict *)ctn)->vtable->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    it = ctn->vtable->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "bar");
    assert_int_equal(it->vtable->previous(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "barz");
    assert_int_equal(it->vtable->previous(it), FALSE);
    assert_non_null(it->vtable->dereference(it));
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 0);
    pair = ((t_data *)it->vtable->dereference(it))->data;
    assert_string_equal(pair->data.data, "barz");
    delete(it);
    delete(ctn);
    (void)state;
}

const struct CMUnitTest iterator_tests[] = {
    cmocka_unit_test_setup_teardown(iterator_empty_array_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_array_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_array_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_array_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_array_arithmetic, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_array_compare_its, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_null_string_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_null_string_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_string_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_string_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_arithmetic, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_compare_its, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_empty_spl_list_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_spl_list_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_spl_list_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_spl_list_end, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_empty_spl_clist_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_spl_clist_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_spl_clist_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_spl_clist_end, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_empty_dbl_list_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_dbl_list_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dbl_list_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dbl_list_end, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_empty_dbl_clist_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_dbl_clist_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dbl_clist_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dbl_clist_end, setup_foo_bar_typed_array, teardown_foo_bar_array),

    cmocka_unit_test_setup_teardown(iterator_empty_dict_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_dict_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dict_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_dict_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
};