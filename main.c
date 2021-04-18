#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"
#include "iterators.h"
#include "dicts.h"
#include "variadic.h"

int     main(int ac, char **av)
{
    (void)ac;
    (void)av;
    Array *array;
    array = new_obj(Array, .to_copy = (t_data *[]){&(t_data){TYPE_CSTRING, "fabinho"}, &(t_data ){TYPE_CSTRING, "HELL0000"}, NULL}, .copy_amount = 1);
    array->dump(array, "Title smile", typed_basic_print, "");
    return (1);
}
