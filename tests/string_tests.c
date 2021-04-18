#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "tests.h"
#include "stringsdef.h"
#include "arrays.h"

static void string_alloc_obj_no_args(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_non_null(str);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_alloc_obj_fully_copy_str(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
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

    str = new_obj(String, .to_copy = "foobar", .copy_amount = 3);
    assert_non_null(str);
    assert_non_null(str->contained);
    assert_string_equal(str->contained, "foo");
    assert_int_equal(str->contained_size, 3);
    delete(str);
    (void)state;
}

static void string_null_string_cstr(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->cstr(str));
    delete(str);
    (void)state;
}

static void string_empty_string_cstr(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_string_equal(str->cstr(str), "");
    delete(str);
    (void)state;
}

static void string_normal_string_cstr(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->cstr(str), "foobar");
    delete(str);
    (void)state;
}

static void string_null_string_size(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->size(str), 0);
    delete(str);
    (void)state;
}

static void string_empty_string_size(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_int_equal(str->size(str), 0);
    delete(str);
    (void)state;
}

static void string_normal_string_size(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->size(str), 6);
    delete(str);
    (void)state;
}

static void string_null_string_empty(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->empty(str), TRUE);
    delete(str);
    (void)state;
}

static void string_empty_string_empty(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_int_equal(str->empty(str), TRUE);
    delete(str);
    (void)state;
}

static void string_normal_string_empty(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->empty(str), FALSE);
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_0_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', 0), TRUE);
    assert_string_equal(str->contained, "afoobar");
    delete(str);
    (void)state;
}

static void string_insert_at_pos_1_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', 1), TRUE);
    assert_string_equal(str->contained, "faoobar");
    delete(str);
    (void)state;
}

static void string_insert_at_last_pos_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', 6), TRUE);
    assert_string_equal(str->contained, "foobara");
    delete(str);
    (void)state;
}

static void string_insert_at_negative_pos(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', -2), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_insert_at_out_of_range(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->insert_at(str, 'a', 7), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_push_back_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_push_back_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "a");
    delete(str);
    (void)state;
}

static void string_push_back_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->push_back(str, 'a'), TRUE);
    assert_string_equal(str->contained, "foobara");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->delete_at(str, 0), FALSE);
    assert_null(str->contained);
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, 0), FALSE);
    assert_string_equal(str->contained, "");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_0_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, 0), TRUE);
    assert_string_equal(str->contained, "oobar");
    delete(str);
    (void)state;
}

static void string_delete_at_pos_3_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, 3), TRUE);
    assert_string_equal(str->contained, "fooar");
    delete(str);
    (void)state;
}

static void string_delete_at_last_pos_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, 5), TRUE);
    assert_string_equal(str->contained, "fooba");
    delete(str);
    (void)state;
}

static void string_delete_at_negative_pos(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, -2), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_delete_at_out_of_range(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->delete_at(str, 6), FALSE);
    assert_string_equal(str->contained, "foobar");
    delete(str);
    (void)state;
}

static void string_erase_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->erase(str), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_erase_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->erase(str), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_affect_null_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->affect(str, NULL), TRUE);
    assert_null(str->contained);
    assert_int_equal(str->contained_size, 0);
    delete(str);
    (void)state;
}

static void string_affect_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->affect(str, "foobarz"), TRUE);
    assert_string_equal(str->contained, "foobarz");
    assert_int_equal(str->contained_size, 7);
    delete(str);
    (void)state;
}

static void string_front_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->front(str));
    delete(str);
    (void)state;
}

static void string_front_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_string_equal(str->front(str), "");
    delete(str);
    (void)state;
}

static void string_front_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->front(str), "foobar");
    delete(str);
    (void)state;
}

static void string_back_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->back(str));
    delete(str);
    (void)state;
}

static void string_back_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_string_equal(str->back(str), "");
    delete(str);
    (void)state;
}

static void string_back_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->back(str), "r");
    delete(str);
    (void)state;
}

static void string_at_pos_0_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->at(str, 0));
    delete(str);
    (void)state;
}

static void string_at_pos_0_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    assert_string_equal(str->at(str, 0), "");
    delete(str);
    (void)state;
}

static void string_at_pos_0_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->at(str, 0), "foobar");
    delete(str);
    (void)state;
}

static void string_at_pos_1_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->at(str, 1), "oobar");
    delete(str);
    (void)state;
}

static void string_at_last_pos_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->at(str, 5), "r");
    delete(str);
    (void)state;
}

static void string_at_negative_pos_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->at(str, -2));
    delete(str);
    (void)state;
}

static void string_at_out_of_range_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->at(str, 6));
    delete(str);
    (void)state;
}

static void string_find_null_str_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->find_str(str, NULL));
    delete(str);
    (void)state;
}

static void string_find_null_str_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->find_str(str, NULL));
    delete(str);
    (void)state;
}

static void string_find_normal_str_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->find_str(str, "bar"));
    delete(str);
    (void)state;
}

static void string_find_normal_str_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->find_str(str, "ba"), "bar");
    delete(str);
    (void)state;
}

static void string_find_1st_occurence_normal_char_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->find(str, 'o'));
    delete(str);
    (void)state;
}

static void string_find_1st_occurence_normal_char_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->find(str, 'o'), "oobar");
    delete(str);
    (void)state;
}

static void string_find_1st_occurence_normal_char_not_present_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->find(str, 'z'));
    delete(str);
    (void)state;
}

static void string_find_last_occurence_normal_char_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->rfind(str, 'o'));
    delete(str);
    (void)state;
}

static void string_find_last_occurence_normal_char_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_string_equal(str->rfind(str, 'o'), "obar");
    delete(str);
    (void)state;
}

static void string_find_last_occurence_normal_char_not_present_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->rfind(str, 'z'));
    delete(str);
    (void)state;
}

static void string_match_null_string_with_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->match(str, NULL), FALSE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_null_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, NULL), FALSE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, ""), FALSE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_non_matching_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "foo"), FALSE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_matching_normal_string_no_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "foobar"), TRUE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_one_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "*"), TRUE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_multiple_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "***"), TRUE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_matching_normal_string_including_one_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "f*"), TRUE);
    delete(str);
    (void)state;
}

static void string_match_normal_string_with_matching_normal_string_including_multiple_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->match(str, "*ba*"), TRUE);
    delete(str);
    (void)state;
}

static void string_nmatch_null_string_with_null_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_int_equal(str->nmatch(str, NULL), -1);
    delete(str);
    (void)state;
}

static void string_nmatch_normal_string_with_null_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, NULL), -1);
    delete(str);
    (void)state;
}

static void string_nmatch_normal_string_with_empty_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, ""), 0);
    delete(str);
    (void)state;
}

static void string_nmatch_normal_string_with_non_matching_normal_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, "foo"), 0);
    delete(str);
    (void)state;
}

static void string_nmatch_normal_string_with_matching_normal_string_no_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, "foobar"), 1);
    delete(str);
    (void)state;
}

static void string_nmatch_normal_string_with_one_wildcard(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, "*"), 1);
    delete(str);
    (void)state;
}

static void string_nmatch_multiple_normal_with_wildcards_tests(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_int_equal(str->nmatch(str, "f*"), 1);
    assert_int_equal(str->nmatch(str, "*r"), 1);
    assert_int_equal(str->nmatch(str, "*b*"), 1);
    assert_int_equal(str->nmatch(str, "*r*"), 1);
    assert_int_equal(str->nmatch(str, "*o*"), 2);
    delete(str);
    (void)state;
}

static void string_split_null_string_with_null_token_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->split(str, BLUEPRINT(Array), NULL));
    delete(str);
    (void)state;
}

static void string_split_null_string_with_empty_token_string(void **state)
{
    String  *str;

    str = new_obj(String);
    assert_null(str->split(str, BLUEPRINT(Array), ""));
    delete(str);
    (void)state;
}

static void string_split_normal_string_with_null_token_string(void **state)
{
    String  *str;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    assert_null(str->split(str, BLUEPRINT(Array), NULL));
    delete(str);
    (void)state;
}

static void     string_split_normal_string_with_empty_token_string(void **state)
{
    String      *str;
    Container   *array;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    array = str->split(str, BLUEPRINT(Array), "");
    assert_non_null(array);
    assert_non_null(array->contained);
    assert_int_equal(array->contained_size, 1);
    assert_string_equal(((t_data **)array->contained)[0]->data, "foobar");
    delete(array);
    delete(str);
    (void)state;
}

static void     string_split_normal_string_with_non_matching_single_char_token_string(void **state)
{
    String      *str;
    Container   *array;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    array = str->split(str, BLUEPRINT(Array), ",");
    assert_non_null(array);
    assert_non_null(array->contained);
    assert_int_equal(array->contained_size, 1);
    assert_string_equal(((t_data **)array->contained)[0]->data, "foobar");
    delete(array);
    delete(str);
    (void)state;
}

static void     string_split_normal_string_with_non_matching_multiple_char_token_string(void **state)
{
    String      *str;
    Container   *array;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    array = str->split(str, BLUEPRINT(Array), ",;");
    assert_non_null(array);
    assert_non_null(array->contained);
    assert_int_equal(array->contained_size, 1);
    assert_string_equal(((t_data **)array->contained)[0]->data, "foobar");
    delete(array);
    delete(str);
    (void)state;
}

static void     string_split_normal_string_with_matching_single_char_token_string(void **state)
{
    String      *str;
    Container   *array;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    array = str->split(str, BLUEPRINT(Array), "o");
    assert_non_null(array);
    assert_non_null(array->contained);
    assert_int_equal(array->contained_size, 2);
    assert_string_equal(((t_data **)array->contained)[0]->data, "f");
    assert_string_equal(((t_data **)array->contained)[1]->data, "bar");
    delete(array);
    delete(str);
    (void)state;
}

static void     string_split_normal_string_with_matching_multiple_char_token_string(void **state)
{
    String      *str;
    Container   *array;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    array = str->split(str, BLUEPRINT(Array), "oa");
    assert_non_null(array);
    assert_non_null(array->contained);
    assert_int_equal(array->contained_size, 3);
    assert_string_equal(((t_data **)array->contained)[0]->data, "f");
    assert_string_equal(((t_data **)array->contained)[1]->data, "b");
    assert_string_equal(((t_data **)array->contained)[2]->data, "r");
    delete(array);
    delete(str);
    (void)state;
}

static void string_sub_null_string_pos_0_len_0(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String);
    sub = str->sub(str, 0, 0);
    assert_null(sub);
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_empty_string_pos_0_len_0(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "", .copy_amount = COPY_ALL);
    sub = str->sub(str, 0, 0);
    assert_string_equal(sub, "");
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_pos_0_len_0(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, 0, 0);
    assert_string_equal(sub, "");
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_out_of_range_pos_len_0(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, 666, 0);
    assert_null(sub);
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_pos_0_too_long_length(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, 0, 666);
    assert_null(sub);
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_pos_0_correct_length(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, 0, 3);
    assert_string_equal(sub, "foo");
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_pos_3_correct_length(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, 3, 3);
    assert_string_equal(sub, "bar");
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_negative_pos_too_long_length(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, -3, 666);
    assert_null(sub);
    free(sub);
    delete(str);
    (void)state;
}

static void string_sub_normal_string_negative_pos_correct_length(void **state)
{
    String  *str;
    char    *sub;

    str = new_obj(String, .to_copy = "foobar", .copy_amount = COPY_ALL);
    sub = str->sub(str, -3, 3);
    assert_string_equal(sub, "bar");
    free(sub);
    delete(str);
    (void)state;
}

const struct CMUnitTest string_tests[] = {
    cmocka_unit_test(string_alloc_obj_no_args),
    cmocka_unit_test(string_alloc_obj_fully_copy_str),
    cmocka_unit_test(string_alloc_obj_partially_copy_str),
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
    cmocka_unit_test(string_front_null_string),
    cmocka_unit_test(string_front_empty_string),
    cmocka_unit_test(string_front_normal_string),
    cmocka_unit_test(string_back_null_string),
    cmocka_unit_test(string_back_empty_string),
    cmocka_unit_test(string_back_normal_string),
    cmocka_unit_test(string_at_pos_0_null_string),
    cmocka_unit_test(string_at_pos_0_empty_string),
    cmocka_unit_test(string_at_pos_0_normal_string),
    cmocka_unit_test(string_at_pos_1_normal_string),
    cmocka_unit_test(string_at_last_pos_normal_string),
    cmocka_unit_test(string_at_negative_pos_normal_string),
    cmocka_unit_test(string_at_out_of_range_normal_string),
    cmocka_unit_test(string_find_null_str_null_string),
    cmocka_unit_test(string_find_null_str_normal_string),
    cmocka_unit_test(string_find_normal_str_null_string),
    cmocka_unit_test(string_find_normal_str_normal_string),
    cmocka_unit_test(string_find_1st_occurence_normal_char_null_string),
    cmocka_unit_test(string_find_1st_occurence_normal_char_normal_string),
    cmocka_unit_test(string_find_1st_occurence_normal_char_not_present_normal_string),
    cmocka_unit_test(string_find_last_occurence_normal_char_null_string),
    cmocka_unit_test(string_find_last_occurence_normal_char_normal_string),
    cmocka_unit_test(string_find_last_occurence_normal_char_not_present_normal_string),
    cmocka_unit_test(string_match_null_string_with_null_string),
    cmocka_unit_test(string_match_normal_string_with_null_string),
    cmocka_unit_test(string_match_normal_string_with_empty_string),
    cmocka_unit_test(string_match_normal_string_with_non_matching_normal_string),
    cmocka_unit_test(string_match_normal_string_with_matching_normal_string_no_wildcard),
    cmocka_unit_test(string_match_normal_string_with_one_wildcard),
    cmocka_unit_test(string_match_normal_string_with_multiple_wildcard),
    cmocka_unit_test(string_match_normal_string_with_matching_normal_string_including_one_wildcard),
    cmocka_unit_test(string_match_normal_string_with_matching_normal_string_including_multiple_wildcard),
    cmocka_unit_test(string_nmatch_null_string_with_null_string),
    cmocka_unit_test(string_nmatch_normal_string_with_null_string),
    cmocka_unit_test(string_nmatch_normal_string_with_empty_string),
    cmocka_unit_test(string_nmatch_normal_string_with_non_matching_normal_string),
    cmocka_unit_test(string_nmatch_normal_string_with_matching_normal_string_no_wildcard),
    cmocka_unit_test(string_nmatch_normal_string_with_one_wildcard),
    cmocka_unit_test(string_nmatch_multiple_normal_with_wildcards_tests),
    cmocka_unit_test(string_split_null_string_with_null_token_string),
    cmocka_unit_test(string_split_null_string_with_empty_token_string),
    cmocka_unit_test(string_split_normal_string_with_null_token_string),
    cmocka_unit_test(string_split_normal_string_with_empty_token_string),
    cmocka_unit_test(string_split_normal_string_with_non_matching_single_char_token_string),
    cmocka_unit_test(string_split_normal_string_with_non_matching_multiple_char_token_string),
    cmocka_unit_test(string_split_normal_string_with_matching_single_char_token_string),
    cmocka_unit_test(string_split_normal_string_with_matching_multiple_char_token_string),
    cmocka_unit_test(string_sub_null_string_pos_0_len_0),
    cmocka_unit_test(string_sub_empty_string_pos_0_len_0),
    cmocka_unit_test(string_sub_normal_string_pos_0_len_0),
    cmocka_unit_test(string_sub_normal_string_out_of_range_pos_len_0),
    cmocka_unit_test(string_sub_normal_string_pos_0_too_long_length),
    cmocka_unit_test(string_sub_normal_string_pos_0_correct_length),
    cmocka_unit_test(string_sub_normal_string_pos_3_correct_length),
    cmocka_unit_test(string_sub_normal_string_negative_pos_too_long_length),
    cmocka_unit_test(string_sub_normal_string_negative_pos_correct_length),
};