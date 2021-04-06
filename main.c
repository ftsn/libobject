#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"
#include "iterators.h"
#include "dicts.h"

#pragma pack(push,1)
typedef struct  s_toto {
    void        (*fptr)();
    int         *n;
    char        *c;
} t_toto;
#pragma pack(pop)

int     main(int ac, char **av)
{
    Container *dict;
    ssize_t i;

    dict = new(_dict);
    i = -1;
    while (++i < ac)
        ((Dict *)dict)->push(dict, (unsigned char *)av[i], &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->remove(dict, (unsigned char *)"hello");
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add1", &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add2", &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add3", &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add4", &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add5", &i, TYPE_INT);
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    ((Dict *)dict)->remove(dict, (unsigned char *)"add5");
    dict->dump(dict, "Dict dump", typed_basic_print, "");
    delete(dict);
    return (1);
}
