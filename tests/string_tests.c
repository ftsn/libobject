#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests.h"
#include "stringsdef.h"

static void string_alloc_obj_no_args(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_non_null(str);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_alloc_obj_fully_copy_str(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_string_equal(str->contained, "foobar");
    assert_int_equal(str->contained_size, 6);
    delete(str);
    (void)state;
}

static void string_alloc_obj_partially_copy_str(void **state)
{
    String  *str;

    str = new(_string, "foobar", 3, 0);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_string_equal(str->contained, "foo");
    assert_int_equal(str->contained_size, 3);
    delete(str);
    (void)state;
}

static void string_alloc_obj_fully_copy_str_plus_additional_args(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 1, 'z');
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_string_equal(str->contained, "foobarz");
    assert_int_equal(str->contained_size, 7);
    delete(str);
    (void)state;
}

static void string_alloc_obj_partially_copy_str_plus_additional_args(void **state)
{
    String  *str;

    str = new(_string, "foobar", 3, 4, 'b', 'a', 'r', 'z');
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_string_equal(str->contained, "foobarz");
    assert_int_equal(str->contained_size, 7);
    delete(str);
    (void)state;
}

static void string_null_string_cstr(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_null(str->cstr(str));
    delete(str);
    (void)state;
}

static void string_empty_string_cstr(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_string_equal(str->cstr(str), "");
    delete(str);
    (void)state;
}

static void string_normal_string_cstr(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_string_equal(str->cstr(str), "foobar");
    delete(str);
    (void)state;
}

static void string_null_string_size(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->size(str), 0);
    delete(str);
    (void)state;
}

static void string_empty_string_size(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_int_equal(str->size(str), 0);
    delete(str);
    (void)state;
}

static void string_normal_string_size(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->size(str), 6);
    delete(str);
    (void)state;
}

static void string_null_string_empty(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->empty(str), TRUE);
    delete(str);
    (void)state;
}

static void string_empty_string_empty(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_int_equal(str->empty(str), TRUE);
    delete(str);
    (void)state;
}

static void string_normal_string_empty(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->empty(str), FALSE);
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_null_string(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_empty_string(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "afoobar");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_1_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', 1), TRUE);
    assert_string_equal(str->contained, "faoobar");
    delete(str);
    (void)state;
}

static void string_insert_at_last_pos_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', 6), TRUE);
    assert_string_equal(str->contained, "foobara");
    delete(str);
    (void)state;
}

static void string_insert_at_negative_pos(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', -2), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_insert_at_out_of_range(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->insert_at(str, 'a', 7), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_push_back_null_string(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_push_back_empty_string(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_push_back_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "foobara");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_null_string(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->delete_at(str, 0), FALSE);
    assert_null(str->contained);
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_empty_string(void **state)
{
    String  *str;

    str = new(_string, "", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, 0), FALSE);
    assert_string_equal(str->contained, "");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, 0), TRUE);
    assert_string_equal(str->contained, "oobar");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_3_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, 3), TRUE);
    assert_string_equal(str->contained, "fooar");
    delete(str);
    (void)state;
}

static void string_delete_at_last_pos_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, 5), TRUE);
    assert_string_equal(str->contained, "fooba");
    delete(str);
    (void)state;
}

static void string_delete_at_negative_pos(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, -2), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_delete_at_out_of_range(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->delete_at(str, 6), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_erase_null_string(void **state)
{
    String  *str;

    str = new(_string, NULL, 0);
    assert_int_equal(str->erase(str), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_erase_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->erase(str), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_affect_null_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->affect(str, NULL), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_affect_normal_string(void **state)
{
    String  *str;

    str = new(_string, "foobar", COPY_ALL, 0);
    assert_int_equal(str->affect(str, "foobarz"), TRUE);
    assert_string_equal(str->contained, "foobarz");
    assert_int_equal(str->contained_size, 7);
    delete(str);
    (void)state;
}

const struct CMUnitTest string_tests[] = {
    cmocka_unit_test(string_alloc_obj_no_args),
    cmocka_unit_test(string_alloc_obj_fully_copy_str),
    cmocka_unit_test(string_alloc_obj_partially_copy_str),
    cmocka_unit_test(string_alloc_obj_fully_copy_str_plus_additional_args),
    cmocka_unit_test(string_alloc_obj_partially_copy_str_plus_additional_args),
    cmocka_unit_test(string_null_string_cstr),
    cmocka_unit_test(string_empty_string_cstr),
    cmocka_unit_test(string_normal_string_cstr),
    cmocka_unit_test(string_null_string_size),
    cmocka_unit_test(string_empty_string_size),
    cmocka_unit_test(string_normal_string_size),
    cmocka_unit_test(string_null_string_empty),
    cmocka_unit_test(string_empty_string_empty),
    cmocka_unit_test(string_normal_string_empty),
    cmocka_unit_test(string_insert_at_pos_0_null_string),
    cmocka_unit_test(string_insert_at_pos_0_empty_string),
    cmocka_unit_test(string_insert_at_pos_0_normal_string),
    cmocka_unit_test(string_insert_at_pos_1_normal_string),
    cmocka_unit_test(string_insert_at_last_pos_normal_string),
    cmocka_unit_test(string_insert_at_negative_pos),
    cmocka_unit_test(string_insert_at_out_of_range),
    cmocka_unit_test(string_push_back_null_string),
    cmocka_unit_test(string_push_back_empty_string),
    cmocka_unit_test(string_push_back_normal_string),
    cmocka_unit_test(string_delete_at_pos_0_null_string),
    cmocka_unit_test(string_delete_at_pos_0_empty_string),
    cmocka_unit_test(string_delete_at_pos_0_normal_string),
    cmocka_unit_test(string_delete_at_pos_3_normal_string),
    cmocka_unit_test(string_delete_at_last_pos_normal_string),
    cmocka_unit_test(string_delete_at_negative_pos),
    cmocka_unit_test(string_delete_at_out_of_range),
    cmocka_unit_test(string_erase_null_string),
    cmocka_unit_test(string_erase_normal_string),
    cmocka_unit_test(string_affect_null_string),
    cmocka_unit_test(string_affect_normal_string),
};