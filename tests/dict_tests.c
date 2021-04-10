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

    ctn = new(_dict, NULL, 0);
    assert_non_null(ctn);
    assert_non_null(ctn->contained);
    assert_int_equal(ctn->contained_size, 0);
    assert_int_not_equal(((Dict *)ctn)->total_size, 0);
    delete(ctn);
    (void)state;
}

const struct CMUnitTest dict_tests[] = {
    cmocka_unit_test(dict_alloc_obj_no_args),
};