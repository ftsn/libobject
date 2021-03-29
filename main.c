#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"

#pragma pack(push,1)
typedef struct  s_toto {
    void        (*fptr)();
    int         *n;
    char        *c;
} t_toto;
#pragma pack(pop)

int     main(int ac, char **av)
{
    Container *a, *b, *c;
    t_data **data_array1, **data_array2;
    String *s;
    char *sub;
    int nb;

    data_array2 = init_typed_array(2);
    data_array2[0] = raw_data_to_typed("Giga recursed test", TYPE_CSTRING);
    data_array2[1] = raw_data_to_typed("?", TYPE_CHAR);
    b = new (_array, data_array2, COPY_ALL, 0);
    b->dump(b, "Array b", typed_basic_print, "");

    nb = 666;
    data_array1 = init_typed_array(4);
    data_array1[0] = raw_data_to_typed("Giga test", TYPE_CSTRING);
    data_array1[1] = raw_data_to_typed(b, TYPE_ARRAY);
    data_array1[2] = raw_data_to_typed(&nb, TYPE_INT);
    data_array1[3] = raw_data_to_typed("!", TYPE_CHAR);
    a = new (_array, data_array1, COPY_ALL, 0);
    a->delete_at(a, 0);
    a->delete_at(a, 2);
    a->dump(a, "Array a", typed_basic_print, "");

    c = new(_dbl_clist, data_array1, COPY_ALL, 0);
    c->dump(c, "Circular doubly linked list c", typed_basic_print, "");

    delete (a);
    delete (b);
    delete(c);
    free_typed_array(data_array1);
    free_typed_array(data_array2);

    if (ac == 2) {
        s = new(_string, av[1], COPY_ALL, 5, 'a', 'b', 'c', 'd', 'e');
        sub = s->sub(s, -5, 3);
        printf("Generating substring from [%s]: [%s]\n", s->cstr(s), sub ? sub : "NULL");
        free(sub);
        printf("[%s]: %zd chars long. %s\n", s->contained ? s->contained : "NULL", s->size(s), s->empty(s) ? "EMPTY" : "NON EMPTY");
        printf("Finding substring bite: [%s]\n", s->find_str(s, "bite"));
        s->insert_at(s, '6', 3);
        s->push_back(s, '7');
        s->delete_at(s, 0);
        s->dump(s, "String: ");
        printf("[%s] [%s] [%s]\n", s->front(s), s->back(s), s->at(s, 3));
        delete(s);
    }


    char *p1 = av[0];
    char *p2 = av[1];
    if (p1 == p2)
        printf("EQUALS\n");
    else
        printf("NOPE\n");
    //(void)toto1;
    //(void)toto2;
    //(void)tata;
    (void)ac;
    (void)av;
    return (1);
}
