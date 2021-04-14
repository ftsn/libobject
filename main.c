#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"
#include "iterators.h"
#include "dicts.h"
#include "variadic.h"

class_definition(Test, TEST, .base = { .__size__ = sizeof(Test), .__init__ = _shallow__test_ctor })
Object      *ctor_definition(TEST) {
    Test    *test;

    test = args->class;
    test->nb = args->nb;
    return (args->class);
}

int     main(int ac, char **av)
{
    (void)ac;
    (void)av;
    Container *array;
    array = new_obj(ARRAY, .to_copy = (t_data *[]){&(t_data){TYPE_CSTRING, "fabinho"}, &(t_data ){TYPE_CSTRING, "HELL0000"}, NULL}, .copy_amount = 1);
    array->dump(array, "Title smile", typed_basic_print, "");
    //((String *)STRING)->dup = &_string_dup;

    Test *test;

    test = new_obj(TEST, .nb=555);
    printf("Test nb: %d %d\n", ((Class *)test)->__type__, test->nb);
    return (1);
}
