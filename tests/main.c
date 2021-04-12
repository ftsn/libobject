#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests.h"

int main(void)
{
    cmocka_run_group_tests(string_tests, NULL, NULL);
    cmocka_run_group_tests(array_tests, NULL, NULL);
    cmocka_run_group_tests(dict_tests, NULL, NULL);
    //cmocka_run_group_tests(list_tests, NULL, NULL);
    //cmocka_run_group_tests(iterator_tests, NULL, NULL);
    return (0);
}
