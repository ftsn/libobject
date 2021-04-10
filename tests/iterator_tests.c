#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "stringsdef.h"
#include "arrays.h"
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

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_empty_array_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_array_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(it->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    assert_int_equal(it->next(it), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_array_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    assert_int_equal(it->previous(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(it->previous(it), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_array_arithmetic(void **state)
{
    Container               *ctn;
    Iterator                *it;
    RandomAccessIterator    *ra;

    ctn = new(_array, *state, COPY_ALL, 2, "baz", TYPE_CSTRING, "barz", TYPE_CSTRING);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 4);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->begin(ctn);
    ra = (RandomAccessIterator *)it;
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(ra->jump(ra, 2), TRUE);
    assert_int_equal(ra->jump(ra, 2), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 2);
    assert_string_equal(((t_data *)it->dereference(it))->data, "baz");
    assert_int_equal(it->next(it), TRUE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 3);
    assert_string_equal(((t_data *)it->dereference(it))->data, "barz");
    assert_int_equal(ra->jump(ra, -3), TRUE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_array_compare_its(void **state)
{
    Container               *ctn;
    Iterator                *it1, *it2;
    RandomAccessIterator    *ra1, *ra2;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it1 = ctn->begin(ctn);
    assert_non_null(it1);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 0);
    assert_string_equal(((t_data *)it1->dereference(it1))->data, "foo");
    it2 = ctn->begin(ctn);
    assert_non_null(it2);
    assert_int_equal(it2->reached_the_beginning, 0);
    assert_int_equal(it2->reached_the_end, 0);
    assert_int_equal(it2->it_idx, 0);
    assert_string_equal(((t_data *)it2->dereference(it2))->data, "foo");
    assert_int_equal(it1->equals(it1, it2), TRUE);
    assert_int_equal(it1->next(it1), TRUE);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 1);
    ra1 = (RandomAccessIterator *)it1;
    ra2 = (RandomAccessIterator *)it2;
    assert_int_equal(ra1->lt(ra1, ra2), FALSE);
    assert_int_equal(ra1->gt(ra1, ra2), TRUE);
    assert_int_equal(ra2->lt(ra2, ra1), TRUE);
    assert_int_equal(ra2->gt(ra2, ra1), FALSE);
    delete(it1);
    delete(it2);
    delete(ctn);
    (void)state;
}








static void     iterator_null_string_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_string, NULL, 0);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->begin(ctn);
    assert_null(it);
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_empty_string_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_string, "", COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    it = ctn->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_empty_string_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 1);
    assert_int_equal(it->it_idx, 0);
    assert_int_equal(((RandomAccessIterator *)it)->ra_idx, 0);
    assert_null(it->dereference(it));
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_string_begin(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->begin(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(it->next(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    assert_int_equal(it->next(it), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    delete(it);
    delete(ctn);
    (void)state;
}

static void     iterator_non_empty_string_end(void **state)
{
    Container   *ctn;
    Iterator    *it;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->end(ctn);
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "bar");
    assert_int_equal(it->previous(it), TRUE);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(it->previous(it), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 1);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 1);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_string_arithmetic(void **state)
{
    Container               *ctn;
    Iterator                *it;
    RandomAccessIterator    *ra;

    ctn = new(_array, *state, COPY_ALL, 2, "baz", TYPE_CSTRING, "barz", TYPE_CSTRING);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 4);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it = ctn->begin(ctn);
    ra = (RandomAccessIterator *)it;
    assert_non_null(it);
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    assert_int_equal(ra->jump(ra, 2), TRUE);
    assert_int_equal(ra->jump(ra, 2), FALSE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 2);
    assert_string_equal(((t_data *)it->dereference(it))->data, "baz");
    assert_int_equal(it->next(it), TRUE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 3);
    assert_string_equal(((t_data *)it->dereference(it))->data, "barz");
    assert_int_equal(ra->jump(ra, -3), TRUE);
    assert_non_null(it->dereference(it));
    assert_int_equal(it->reached_the_beginning, 0);
    assert_int_equal(it->reached_the_end, 0);
    assert_int_equal(it->it_idx, 0);
    assert_string_equal(((t_data *)it->dereference(it))->data, "foo");
    delete(it);
    delete(ctn);
    (void)state;
}

static void                 iterator_non_empty_string_compare_its(void **state)
{
    Container               *ctn;
    Iterator                *it1, *it2;
    RandomAccessIterator    *ra1, *ra2;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    it1 = ctn->begin(ctn);
    assert_non_null(it1);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 0);
    assert_string_equal(((t_data *)it1->dereference(it1))->data, "foo");
    it2 = ctn->begin(ctn);
    assert_non_null(it2);
    assert_int_equal(it2->reached_the_beginning, 0);
    assert_int_equal(it2->reached_the_end, 0);
    assert_int_equal(it2->it_idx, 0);
    assert_string_equal(((t_data *)it2->dereference(it2))->data, "foo");
    assert_int_equal(it1->equals(it1, it2), TRUE);
    assert_int_equal(it1->next(it1), TRUE);
    assert_int_equal(it1->reached_the_beginning, 0);
    assert_int_equal(it1->reached_the_end, 0);
    assert_int_equal(it1->it_idx, 1);
    ra1 = (RandomAccessIterator *)it1;
    ra2 = (RandomAccessIterator *)it2;
    assert_int_equal(ra1->lt(ra1, ra2), FALSE);
    assert_int_equal(ra1->gt(ra1, ra2), TRUE);
    assert_int_equal(ra2->lt(ra2, ra1), TRUE);
    assert_int_equal(ra2->gt(ra2, ra1), FALSE);
    delete(it1);
    delete(it2);
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
    cmocka_unit_test_setup_teardown(iterator_empty_string_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_empty_string_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_begin, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_end, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_arithmetic, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(iterator_non_empty_string_compare_its, setup_foo_bar_typed_array, teardown_foo_bar_array),
};