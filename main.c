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

class_definition(Test, TEST,
    class_metadata(Test, TEST, TYPE_ABSTRACT, NULL),
    12
)
Object      *ctor_definition(TEST) {
    Test    *test;

    test = args->class;
    test->nb = args->nb;
    return (args->class);
}

typedef struct {
    int z;
    int k;
} gr8parent;

typedef struct {
    gr8parent;
    int y;
} parent;

struct child {
    parent;
    int toto;
};

struct child childzer;

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


    gr8parent gr8 = {100, 200};
    unsigned char *bar = (void *)&gr8;
    printf("%d\n", *(int *)(bar + offsetof( gr8parent, k )));
    return (1);
}
