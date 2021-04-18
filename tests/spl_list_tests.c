#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "lists.h"

static void     *list_data_lookup(Container *ctn, ssize_t pos)
{
    t_list_data *list;

    list = ctn->contained;
    for (ssize_t i = 0; i < pos; ++i)
        list = list->next;
    return (((t_data *)list->data)->data);
}

static int setup_foo_bar_typed_array(void **state)
{
    t_data **data_tab;

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

static void spl_list_alloc_obj_no_args(void **state)
{

    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete (ctn);

    (void)state;
}

static void spl_list_alloc_obj_fully_copy_string_table(void **state)
{

    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_alloc_obj_partially_copy_string_table(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = 1);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    delete (ctn);
    (void)state;
}

static void spl_list_data(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_ptr_equal(ctn->data(ctn), ctn->contained);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_test_size(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->size(ctn), 0);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_test_size(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->size(ctn), 2);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_test_empty(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->empty(ctn), TRUE);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_test_empty(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->empty(ctn), FALSE);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_insert_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_insert_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_insert_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, -1), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_insert_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 666), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_insert_at_pos_0(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(list_data_lookup(ctn, 0), "foobar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_insert_at_pos_0(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 0), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(list_data_lookup(ctn, 0), "foobar");
    assert_string_equal(list_data_lookup(ctn, 1), "foo");
    assert_string_equal(list_data_lookup(ctn, 2), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_insert_at_pos_1(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 1), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "foobar");
    assert_string_equal(list_data_lookup(ctn, 2), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_insert_at_last_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->insert_at(ctn, "foobar", TYPE_CSTRING, 2), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "bar");
    assert_string_equal(list_data_lookup(ctn, 2), "foobar");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_push_back(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(list_data_lookup(ctn, 0), "foobar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_push_back(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->push_back(ctn, "foobar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    assert_string_equal(list_data_lookup(ctn, 1), "bar");
    assert_string_equal(list_data_lookup(ctn, 2), "foobar");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_delete_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->contained);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_delete_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->contained);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_delete_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, -1), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_delete_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 666), FALSE);
    assert_int_equal(ctn->contained_size, 2);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_delete_at_pos_0(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 0), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(list_data_lookup(ctn, 0), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_delete_at_pos_1(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_int_equal(ctn->delete_at(ctn, 1), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_string_equal(list_data_lookup(ctn, 0), "foo");
    delete (ctn);
    (void)state;
}

static void spl_list_mixed_insertion_deletion(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
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
    delete (ctn);
    (void)state;
}

static void spl_list_empty_erase(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_equal(ctn->erase(ctn), TRUE);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_erase(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_equal(ctn->erase(ctn), TRUE);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    delete (ctn);
    (void)state;
}

static void spl_list_empty_front(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->front(ctn));
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_front(void **state)
{
    Container *ctn;
    t_list_data *node;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    node = ctn->front(ctn);
    assert_string_equal(((t_data *)node->data)->data, "foo");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_back(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->back(ctn));
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_back(void **state)
{
    Container *ctn;
    t_list_data *node;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    node = ctn->back(ctn);
    assert_string_equal(((t_data *)node->data)->data, "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, -1));
    delete (ctn);
    (void)state;
}

static void spl_list_empty_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, 666));
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_at_negative_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_null(ctn->at(ctn, -1));
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_at_out_of_range_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_null(ctn->at(ctn, 666));
    delete (ctn);
    (void)state;
}

static void spl_list_empty_at_pos_0(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_null(ctn->at(ctn, 0));
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_at_pos_0(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(NODE_TO_DATA(ctn->at(ctn, 0)), "foo");
    delete (ctn);
    (void)state;
}

static void spl_list_non_empty_at_last_pos(void **state)
{
    Container *ctn;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    assert_string_equal(NODE_TO_DATA(ctn->at(ctn, 1)), "bar");
    delete (ctn);
    (void)state;
}

static void spl_list_empty_convert_to_lists(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->convert(ctn, BLUEPRINT(SplList));
    assert_non_null(converted);
    assert_null(converted->contained);
    assert_int_equal(converted->contained_size, 0);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_convert_to_lists(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->convert(ctn, BLUEPRINT(SplList));
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_empty_sub_to_array_out_of_range_negative_pos_correct_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -666, 0);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_empty_sub_to_array_correct_pos_too_long_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 1);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_empty_sub_to_array_out_of_range_negative_pos_too_long_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -666, 1);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_empty_sub_to_array_out_of_range_pos_correct_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 666, 0);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_out_of_range_negative_pos_correct_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -666, 1);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_correct_pos_too_long_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 3);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_out_of_range_negative_pos_too_long_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -666, 3);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_out_of_range_pos_correct_length(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 666, 1);
    assert_null(converted);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_empty_sub_to_array_pos_0_len_0(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 0);
    assert_non_null(converted);
    assert_null(converted->contained);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_pos_0_len_0(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 0);
    assert_non_null(converted);
    assert_null(converted->contained);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_pos_0_len_1(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(list_data_lookup(converted, 0), "foo");
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_pos_1_len_1(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 1, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(list_data_lookup(converted, 0), "bar");
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_copy_all(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), 0, 2);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    assert_string_equal(list_data_lookup(converted, 0), "foo");
    assert_string_equal(list_data_lookup(converted, 1), "bar");
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_correct_negative_pos_minus_2_len_1(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -2, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(list_data_lookup(converted, 0), "foo");
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_correct_negative_pos_minus_1_len_1(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -1, 1);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 1);
    assert_string_equal(list_data_lookup(converted, 0), "bar");
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_sub_to_array_correct_negative_pos_minus_2_copy_all(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->sub(ctn, BLUEPRINT(SplList), -2, 2);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    assert_string_equal(list_data_lookup(converted, 0), "foo");
    assert_string_equal(list_data_lookup(converted, 1), "bar");
    delete (ctn);
    delete (converted);
    (void)state;
}

// Map function used to test the map method. Assumes the provided data is a pointer to a non empty string of char.
static t_data map_fn(ssize_t idx, void *cur)
{
    t_data *data;

    data = cur;
    (void)idx;
    return ((t_data){TYPE_CSTRING, ((char *)data->data) + 1});
}

static void spl_list_empty_map_to_lists(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList);
    assert_non_null(ctn);
    assert_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    converted = ctn->map(ctn, BLUEPRINT(SplList), map_fn);
    assert_non_null(converted);
    assert_null(converted->contained);
    assert_int_equal(converted->contained_size, 0);
    delete (ctn);
    delete (converted);
    (void)state;
}

static void spl_list_non_empty_map_to_lists(void **state)
{
    Container *ctn;
    Container *converted;

    ctn = new_obj(SplList, .to_copy = *state, .copy_amount = COPY_ALL);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 2);
    converted = ctn->map(ctn, BLUEPRINT(SplList), map_fn);
    assert_non_null(converted);
    assert_non_null(converted->contained);
    assert_int_equal(converted->contained_size, 2);
    assert_string_equal(list_data_lookup(converted, 0), "oo");
    assert_string_equal(list_data_lookup(converted, 1), "ar");
    delete (ctn);
    delete (converted);
    (void)state;
}

const struct CMUnitTest spl_list_tests[] = {
    cmocka_unit_test_setup_teardown(spl_list_alloc_obj_no_args, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_alloc_obj_fully_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_alloc_obj_partially_copy_string_table, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_data, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_test_size, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_test_empty, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_insert_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_insert_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_insert_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_insert_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_insert_at_last_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_push_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_delete_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_delete_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_delete_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_delete_at_pos_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_mixed_insertion_deletion, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_erase, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_front, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_back, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_at_negative_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_at_out_of_range_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_at_pos_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_at_last_pos, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_convert_to_lists, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_convert_to_lists, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_sub_to_array_out_of_range_negative_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_sub_to_array_correct_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_sub_to_array_out_of_range_negative_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_sub_to_array_out_of_range_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_out_of_range_negative_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_correct_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_out_of_range_negative_pos_too_long_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_out_of_range_pos_correct_length, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_sub_to_array_pos_0_len_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_pos_0_len_0, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_pos_0_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_pos_1_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_copy_all, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_correct_negative_pos_minus_2_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_correct_negative_pos_minus_1_len_1, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_sub_to_array_correct_negative_pos_minus_2_copy_all, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_empty_map_to_lists, setup_foo_bar_typed_array, teardown_foo_bar_array),
    cmocka_unit_test_setup_teardown(spl_list_non_empty_map_to_lists, setup_foo_bar_typed_array, teardown_foo_bar_array),
};