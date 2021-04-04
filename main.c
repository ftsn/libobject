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
    Container *b, *c, *dict;
    Object  *cast_a;
    Array a;
    t_data **data_array1, **data_array2;
    String *s;
    String *t;
    ssize_t i;
    char *sub;
    int nb;

    data_array2 = init_typed_array(2);
    data_array2[0] = raw_data_to_typed("Giga recursed test", TYPE_CSTRING);
    data_array2[1] = raw_data_to_typed("?", TYPE_CSTRING);
    b = new (_array, data_array2, COPY_ALL, 0);
    b->dump(b, "Array b", typed_basic_print, "");

    nb = 666;
    data_array1 = init_typed_array(4);
    data_array1[0] = raw_data_to_typed("Giga test", TYPE_CSTRING);
    data_array1[1] = raw_data_to_typed(b, TYPE_ARRAY);
    data_array1[2] = raw_data_to_typed(&nb, TYPE_INT);
    data_array1[3] = raw_data_to_typed("!", TYPE_CHAR);
    
    static_new(_array, (Class *)&a, data_array1, COPY_ALL, 0);
    cast_a = &a;
    ((Container *)cast_a)->delete_at(cast_a, 0);
    ((Container *)cast_a)->delete_at(cast_a, 2);
    ((Container *)cast_a)->dump(cast_a, "Array a", typed_basic_print, "");


    c = new(_dbl_clist, data_array1, COPY_ALL, 0);
    c->dump(c, "Circular doubly linked list c", typed_basic_print, "");

    static_delete(&a);
    delete (b);
    delete(c);
    free_typed_array(data_array1);

    if (ac == 2) {
        Iterator    *it;

        s = new(_string, av[1], COPY_ALL, 5, 'a', 'b', 'c', 'd', 'e');
        it = generate_it(s, END);
        while (!it->reached_the_beginning)
        {
            printf("%c", *(char *)it->dereference(it));
            it->previous(it);
        }
        delete(it);
        printf("\n");
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

    dict = new(_dict);
    i = -1;
    while (++i < ac)
        ((Dict *)dict)->push(dict, (unsigned char *)av[i], &i, TYPE_INT);
    i = -1;
    Container *list;
    t_data *front;
    t_pair *pair;
    while (++i < ((Dict *)dict)->total_size)
    {
        list = ((void **)dict->contained)[i];
        if (list != NULL)
        {
            front = ((t_list_data *)list->front(list))->data;
            pair = front->data;
            printf("%zd: [%s] => [%d]\n", i, pair->key, *(int *)(pair->data.data));
        }
        else
            printf("%zd: [%p]\n", i, list);
    }

    (void)nb;
    (void)c;
    (void)t;
    (void)sub;
    (void)s;
    (void)a;
    (void)data_array1;
    (void)c;
    (void)data_array2;
    (void)b;
    free_typed_array(data_array2);
    return (1);
}
