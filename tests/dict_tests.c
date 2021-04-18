#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "dicts.h"

static void     dict_alloc_obj_no_args(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

static void     dict_insert_key(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");
    delete(ctn);
    (void)state;
}

static void     dict_insert_key_twice(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");
    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "barz", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "barz");
    delete(ctn);
    (void)state;
}

static void     dict_insert_different_keys(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"bar", "foo", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "bar"), "foo");

    delete(ctn);
    (void)state;
}

static void     dict_mixed_insertion_deletion(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"foo"), TRUE);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_null(DICT_PAIR_LOOKUP(ctn, "foo"));
    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"foo"), FALSE);

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "barz", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "barz");

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"foo"), TRUE);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_null(DICT_PAIR_LOOKUP(ctn, "foo"));

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"push1", "data1", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "push1"), "data1");

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"push2", "data2", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "push2"), "data2");

    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"push3", "data3", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 3);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "push3"), "data3");

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"wrongKey"), FALSE);
    assert_int_equal(ctn->contained_size, 3);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push1"));
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push2"));
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push3"));

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"push2"), TRUE);
    assert_int_equal(ctn->contained_size, 2);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push1"));
    assert_null(DICT_PAIR_LOOKUP(ctn, "push2"));
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push3"));

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"push3"), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_non_null(DICT_PAIR_LOOKUP(ctn, "push1"));
    assert_null(DICT_PAIR_LOOKUP(ctn, "push2"));
    assert_null(DICT_PAIR_LOOKUP(ctn, "push3"));

    assert_int_equal(((Dict *)ctn)->remove(ctn, (unsigned char *)"push1"), TRUE);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_null(DICT_PAIR_LOOKUP(ctn, "push1"));
    assert_null(DICT_PAIR_LOOKUP(ctn, "push2"));
    assert_null(DICT_PAIR_LOOKUP(ctn, "push3"));

    delete(ctn);
    (void)state;
}

static void     dict_data(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_ptr_equal(ctn->data(ctn), ctn->contained);
    delete(ctn);
    (void)state;
}

static void     dict_empty_size(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(ctn->size(ctn), 0);
    delete(ctn);
    (void)state;
}

static void     dict_non_empty_size(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");
    assert_int_equal(ctn->size(ctn), 1);
    delete(ctn);
    (void)state;
}

static void     dict_empty_test_empty(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(ctn->empty(ctn), TRUE);
    delete(ctn);
    (void)state;
}

static void     dict_non_empty_test_empty(void **state)
{
    Container   *ctn;

    ctn = new_obj(Dict);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_int_equal(((Dict *)ctn)->push(ctn, (unsigned char *)"foo", "bar", TYPE_CSTRING), TRUE);
    assert_int_equal(ctn->contained_size, 1);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    assert_string_equal(DICT_DATA_LOOKUP(ctn, "foo"), "bar");
    assert_int_equal(ctn->empty(ctn), FALSE);
    delete(ctn);
    (void)state;
}

const struct CMUnitTest dict_tests[] = {
    cmocka_unit_test(dict_alloc_obj_no_args),
    cmocka_unit_test(dict_insert_key),
    cmocka_unit_test(dict_insert_key_twice),
    cmocka_unit_test(dict_insert_different_keys),
    cmocka_unit_test(dict_mixed_insertion_deletion),
    cmocka_unit_test(dict_data),
    cmocka_unit_test(dict_empty_size),
    cmocka_unit_test(dict_non_empty_size),
    cmocka_unit_test(dict_empty_test_empty),
    cmocka_unit_test(dict_non_empty_test_empty),
};