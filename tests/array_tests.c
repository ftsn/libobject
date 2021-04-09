#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "arrays.h"

#include <stdio.h>

static int setup_foo_bar_typed_array(void **state)
{
    t_data  **data_tab;

    data_tab = cstrings_to_data_array((char *[]){"foo", "bar", NULL});
    *state = data_tab;
    return (0);
}

static int teardown_foo_bar_array(void **state)
{
    free(*state);
    *state = NULL;
    return (0);
}

static void     array_alloc_obj_no_args(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_alloc_obj_fully_copy_string_table(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_alloc_obj_partially_copy_string_table(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, 1, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_alloc_obj_fully_copy_string_table_plus_additional_args(void **state)
{
    Container   *ctn;
    int         nb;

    nb = 666;
    ctn = new(_array, *state, COPY_ALL, 2, "a", TYPE_CHAR, &nb, TYPE_INT);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 4);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "a");
    assert_int_equal(*(int *)((t_data **)ctn->contained)[3]->data, 666);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_alloc_obj_partially_copy_string_table_plus_additional_args(void **state)
{
    Container   *ctn;
    int         nb;

    nb = 666;
    ctn = new(_array, *state, 1, 2, "a", TYPE_CHAR, &nb, TYPE_INT);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "a");
    assert_int_equal(*(int *)((t_data **)ctn->contained)[2]->data, 666);
    assert_int_not_equal(((Array *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_data(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_ptr_equal(ctn->data(ctn), ctn->contained);
    delete(ctn);
    (void)state;
}

static void     array_empty_test_size(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->size(ctn), 0);
    delete(ctn);
    (void)state;
}

static void     array_non_empty_test_size(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->size(ctn), 2);
    delete(ctn);
    (void)state;
}

static void     array_empty_test_empty(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->empty(ctn), TRUE);
    delete(ctn);
    (void)state;
}

static void     array_non_empty_test_empty(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->empty(ctn), FALSE);
    delete(ctn);
    (void)state;
}

static void     array_empty_insert_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(((void **)ctn->contained)[0]);
    delete(ctn);
    (void)state;
}

static void     array_empty_insert_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(((void **)ctn->contained)[0]);
    delete(ctn);
    (void)state;
}

static void     array_non_empty_insert_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_insert_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_empty_insert_at_pos_0(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foobar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_insert_at_pos_0(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foobar");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_insert_at_pos_1(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 1), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "foobar");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_insert_at_last_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 2), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "foobar");
    delete(ctn);
    (void)state;
}

static void     array_empty_push_back(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foobar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_push_back(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "foobar");
    delete(ctn);
    (void)state;
}

static void     array_empty_delete_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(((void **)ctn->contained)[0]);
    delete(ctn);
    (void)state;
}

static void     array_empty_delete_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(((void **)ctn->contained)[0]);
    delete(ctn);
    (void)state;
}

static void     array_non_empty_delete_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_delete_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_delete_at_pos_0(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_delete_at_pos_1(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 1), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    delete(ctn);
    (void)state;
}

static void     array_mixed_insertion_deletion(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 1), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 1), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "foobar");
    assert_int_equal(ctn->insert_at(ctn, "barfoo", TYPE_CSTRING, 1), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "barfoo");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "foobar");
    assert_int_equal(ctn->push_back(ctn, "baz", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 4);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "barfoo");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "foobar");
    assert_string_equal(((t_data **)ctn->contained)[3]->data, "baz");
    assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
    assert_int_equal(ctn->contained_size, 4);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "barfoo");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "foobar");
    assert_string_equal(((t_data **)ctn->contained)[3]->data, "baz");
    assert_int_equal(ctn->delete_at(ctn, 2), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "barfoo");
    assert_string_equal(((t_data **)ctn->contained)[2]->data, "baz");
    assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "barfoo");
    assert_string_equal(((t_data **)ctn->contained)[1]->data, "baz");
    assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(((t_data **)ctn->contained)[0]->data, "baz");
    assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->delete_at(ctn, 0), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_empty_erase(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->erase(ctn), TRUE);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete(ctn);
    (void)state;
}

static void     array_non_empty_erase(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->erase(ctn), TRUE);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(((t_data **)ctn->contained)[0]);
    delete(ctn);
    (void)state;
}

static void     array_empty_front(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->front(ctn));
    delete(ctn);
    (void)state;
}

static void     array_non_empty_front(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data *)ctn->front(ctn))->data, "foo");
    delete(ctn);
    (void)state;
}

static void     array_empty_back(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->back(ctn));
    delete(ctn);
    (void)state;
}

static void     array_non_empty_back(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data *)ctn->back(ctn))->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_empty_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, -1));
    delete(ctn);
    (void)state;
}

static void     array_empty_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, 666));
    delete(ctn);
    (void)state;
}

static void     array_non_empty_at_negative_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_null(ctn->at(ctn, -1));
    delete(ctn);
    (void)state;
}

static void     array_non_empty_at_out_of_range_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_null(ctn->at(ctn, 666));
    delete(ctn);
    (void)state;
}

static void     array_empty_at_pos_0(void **state)
{
    Container   *ctn;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, 0));
    delete(ctn);
    (void)state;
}

static void     array_non_empty_at_pos_0(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data *)ctn->at(ctn, 0))->data, "foo");
    delete(ctn);
    (void)state;
}

static void     array_non_empty_at_last_pos(void **state)
{
    Container   *ctn;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(((t_data *)ctn->at(ctn, 1))->data, "bar");
    delete(ctn);
    (void)state;
}

static void     array_empty_convert_to_array(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->convert(ctn, _array);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 0);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_convert_to_array(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->convert(ctn, _array);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_empty_sub_to_array_out_of_range_negative_pos_correct_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, _array, -666, 0);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_empty_sub_to_array_correct_pos_too_long_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, _array, 0, 1);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_empty_sub_to_array_out_of_range_negative_pos_too_long_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, _array, -666, 1);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_empty_sub_to_array_out_of_range_pos_correct_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, _array, 666, 1);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_out_of_range_negative_pos_correct_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, -666, 1);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_correct_pos_too_long_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 0, 3);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_out_of_range_negative_pos_too_long_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, -666, 3);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_out_of_range_pos_correct_length(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 666, 1);
    assert_null(converted);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_empty_sub_to_array_pos_0_len_0(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, _array, 0, 0);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 0);
    assert_null(((void **)converted->contained)[0]);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_pos_0_len_0(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 0, 0);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 0);
    assert_null(((void **)converted->contained)[0]);
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_pos_0_len_1(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 0, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "foo");
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_pos_1_len_1(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 1, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "bar");
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_copy_all(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, 0, 2);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)converted->contained)[1]->data, "bar");
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_correct_negative_pos_minus_2_len_1(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, -2, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "foo");
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_correct_negative_pos_minus_1_len_1(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, -1, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "bar");
    delete(ctn);
    delete(converted);
    (void)state;
}

static void     array_non_empty_sub_to_array_correct_negative_pos_minus_2_copy_all(void **state)
{
    Container   *ctn, *converted;

    ctn = new(_array, *state, COPY_ALL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, _array, -2, 2);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    assert_string_equal(((t_data **)converted->contained)[0]->data, "foo");
    assert_string_equal(((t_data **)converted->contained)[1]->data, "bar");
    delete(ctn);
    delete(converted);
    (void)state;
}

static

const struct CMUnitTest array_tests[] = {
    cmocka_unit_test_setup_teardown(array_alloc_obj_no_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_alloc_obj_fully_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_alloc_obj_partially_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_alloc_obj_fully_copy_string_table_plus_additional_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_alloc_obj_partially_copy_string_table_plus_additional_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_data, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_insert_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_insert_at_last_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_delete_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_delete_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_mixed_insertion_deletion, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_at_last_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_convert_to_array, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_convert_to_array, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_sub_to_array_out_of_range_negative_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_sub_to_array_correct_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_sub_to_array_out_of_range_negative_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_sub_to_array_out_of_range_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_out_of_range_negative_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_correct_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_out_of_range_negative_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_out_of_range_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_empty_sub_to_array_pos_0_len_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_pos_0_len_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_pos_0_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_pos_1_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_copy_all, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_correct_negative_pos_minus_2_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_correct_negative_pos_minus_1_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(array_non_empty_sub_to_array_correct_negative_pos_minus_2_copy_all, setup_foo_bar_typed_array, teardown_foo_bar_array),
};
/*
cmocka_unit_test_setup_teardown(, setup_foo_bar_typed_array, teardown_foo_bar_array),
    */