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

#define declare_type(type)  \
        typedef struct                      \
        {                                   \
            CAT(type,_fields)     \
        } type;

typedef struct
{
    size_t  size;
} TestClass;

#define child_fields    \
        int toto;
#define child_def       \
        1
typedef struct
{
    TestClass base;
    child_fields
} child;

#define grand_child_fields  \
        child_fields        \
        int tata;
#define grand_child_def     \
        child_def,          \
        2
typedef struct
{
    TestClass base;
    grand_child_fields
} grand_child;

grand_child foobar = {
    {
        666
    },
    grand_child_def
};

#define child4_fields      \
        grand_child_fields  \
        int zbleeeh;
declare_type(child4)
// Pour une class n + 1 on a besoin 

class_definition(Test,
    class_metadata(Test, TYPE_ABSTRACT, NULL),
    12
)
Object      *ctor_definition(Test) {
    Test    *test;

    test = args->class;
    test->nb = args->nb;
    return (args->class);
}

int     main(int ac, char **av)
{
    (void)ac;
    (void)av;
    Array *array;
    array = new_obj(Array, .to_copy = (t_data *[]){&(t_data){TYPE_CSTRING, "fabinho"}, &(t_data ){TYPE_CSTRING, "HELL0000"}, NULL}, .copy_amount = 1);
    array->dump(array, "Title smile", typed_basic_print, "");
    //((String *)STRING)->dup = &_string_dup;

    Test *test;

    test = new_obj(Test, .nb=555);
    printf("Test nb: %d %d\n", ((Class *)test)->__type__, test->nb);
    return (1);
}
