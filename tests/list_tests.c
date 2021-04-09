#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "lists.h"

#include <stdio.h>

static void     *list_data_lookup(Container *ctn, ssize_t pos)
{
    t_list_data *list;

    list = ctn->contained;
    for (ssize_t i = 0; i < pos; ++i)
        list = list->next;
    return (((t_data *)list->data)->data);
}

static int      setup_foo_bar_typed_array(void **state)
{
    t_data      **data_tab;

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

static void lists_alloc_obj_no_args(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container *ctn;

        ctn = new (kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        delete (ctn);
    );
    (void)state;
}

static void lists_alloc_obj_fully_copy_string_table(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 2);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        delete(ctn);
    );
    (void)state;
}

static void lists_alloc_obj_partially_copy_string_table(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, 1, 0);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        delete(ctn);
    );
    (void)state;
}

static void lists_alloc_obj_fully_copy_string_table_plus_additional_args(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;
        int         nb = 666;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 2, "a", TYPE_CHAR, &nb, TYPE_INT);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 4);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        assert_string_equal(list_data_lookup(ctn, 2), "a");
        assert_int_equal(*(int *)list_data_lookup(ctn, 3), 666);
        delete(ctn);
    );
    (void)state;
}

static void lists_alloc_obj_partially_copy_string_table_plus_additional_args(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;
        int         nb;

        nb = 666;
        ctn = new(kinds[kinds_idx], *state, 1, 2, "a", TYPE_CHAR, &nb, TYPE_INT);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "a");
        assert_int_equal(*(int *)list_data_lookup(ctn, 2), 666);
        delete(ctn);
    );
    (void)state;
}

static void  lists_data(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_ptr_equal(ctn->data(ctn), ctn->contained);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_test_size(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->contained_size, 0);
        assert_int_equal(ctn->size(ctn), 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_test_size(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->contained_size, 2);
        assert_int_equal(ctn->size(ctn), 2);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_test_empty(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->contained_size, 0);
        assert_int_equal(ctn->empty(ctn), TRUE);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_test_empty(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->contained_size, 2);
        assert_int_equal(ctn->empty(ctn), FALSE);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_insert_at_negative_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_insert_at_out_of_range_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_insert_at_negative_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
        assert_int_equal(ctn->contained_size, 2);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_insert_at_out_of_range_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
        assert_int_equal(ctn->contained_size, 2);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_insert_at_pos_0(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "foobar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_insert_at_pos_0(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foobar");
        assert_string_equal(list_data_lookup(ctn, 1), "foo");
        assert_string_equal(list_data_lookup(ctn, 2), "bar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_insert_at_pos_1(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 1), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "foobar");
        assert_string_equal(list_data_lookup(ctn, 2), "bar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_insert_at_last_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 2), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        assert_string_equal(list_data_lookup(ctn, 2), "foobar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_push_back(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "foobar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_push_back(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "bar");
        assert_string_equal(list_data_lookup(ctn, 2), "foobar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_delete_at_negative_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
        assert_int_equal(ctn->contained_size, 0);
        assert_null(ctn->contained);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_delete_at_out_of_range_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
        assert_int_equal(ctn->contained_size, 0);
        assert_null(ctn->contained);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_delete_at_negative_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
        assert_int_equal(ctn->contained_size, 2);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_delete_at_out_of_range_pos(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
        assert_int_equal(ctn->contained_size, 2);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_delete_at_pos_0(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "bar");
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_delete_at_pos_1(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, 1), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        delete(ctn);
    );
    (void)state;
}

static void  lists_mixed_insertion_deletion(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_int_equal(ctn->delete_at(ctn, 1), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 1), TRUE);
        assert_int_equal(ctn->contained_size, 2);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "foobar");
        assert_int_equal(ctn->insert_at(ctn, "barfoo", TYPE_CSTRING, 1), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "barfoo");
        assert_string_equal(list_data_lookup(ctn, 2), "foobar");
        assert_int_equal(ctn->push_back(ctn, "baz", TYPE_CSTRING), TRUE);
        assert_int_equal(ctn->contained_size, 4);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "barfoo");
        assert_string_equal(list_data_lookup(ctn, 2), "foobar");
        assert_string_equal(list_data_lookup(ctn, 3), "baz");
        assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
        assert_int_equal(ctn->contained_size, 4);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "barfoo");
        assert_string_equal(list_data_lookup(ctn, 2), "foobar");
        assert_string_equal(list_data_lookup(ctn, 3), "baz");
        assert_int_equal(ctn->delete_at(ctn, 2), TRUE);
        assert_int_equal(ctn->contained_size, 3);
        assert_string_equal(list_data_lookup(ctn, 0), "foo");
        assert_string_equal(list_data_lookup(ctn, 1), "barfoo");
        assert_string_equal(list_data_lookup(ctn, 2), "baz");
        assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
        assert_int_equal(ctn->contained_size, 2);
        assert_string_equal(list_data_lookup(ctn, 0), "barfoo");
        assert_string_equal(list_data_lookup(ctn, 1), "baz");
        assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
        assert_int_equal(ctn->contained_size, 1);
        assert_string_equal(list_data_lookup(ctn, 0), "baz");
        assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
        assert_int_equal(ctn->contained_size, 0);
        assert_int_equal(ctn->delete_at(ctn, 0), FALSE);
        assert_int_equal(ctn->contained_size, 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_erase(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        assert_int_equal(ctn->erase(ctn), TRUE);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_erase(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 2);
        assert_int_equal(ctn->erase(ctn), TRUE);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_front(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        assert_null(ctn->front(ctn));
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_front(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;
        t_list_data *node;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 2);
        node = ctn->front(ctn);
        assert_string_equal(((t_data *)node->data)->data, "foo");
        delete(ctn);
    );
    (void)state;
}

static void  lists_empty_back(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
        assert_non_null(ctn);
        assert_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 0);
        assert_null(ctn->back(ctn));
        delete(ctn);
    );
    (void)state;
}

static void  lists_non_empty_back(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;
        t_list_data *node;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
        assert_non_null(ctn);
        assert_non_null(ctn->contained);
        assert_int_equal(ctn->contained_size, 2);
        node = ctn->back(ctn);
        assert_string_equal(((t_data *)node->data)->data, "bar");
        delete(ctn);
    );
    (void)state;
}

const struct CMUnitTest list_tests[] = {
    cmocka_unit_test_setup_teardown(lists_alloc_obj_no_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_alloc_obj_fully_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_alloc_obj_partially_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_alloc_obj_fully_copy_string_table_plus_additional_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_alloc_obj_partially_copy_string_table_plus_additional_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_data, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_insert_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_insert_at_last_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_delete_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_delete_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_mixed_insertion_deletion, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(lists_non_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
};
// cmocka_unit_test_setup_teardown(, setup_foo_bar_typed_array, teardown_foo_bar_array),
/*

static void  lists_data(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], NULL, 0);
    );
    (void)state;
}

static void  lists_data(void **state)
{
    LOOP_OVER_ALL_KINDS(
        Container   *ctn;

        ctn = new(kinds[kinds_idx], *state, COPY_ALL, 0);
    );
    (void)state;
}


*/