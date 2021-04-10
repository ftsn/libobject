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

t_data   mapinho(ssize_t idx, void *cur)
{
    t_data  *data;

    data = cur;
    (void)idx;
    return ((t_data){TYPE_CSTRING, ((char *)data->data) + 1  });
}

int     main(int ac, char **av)
{
    Container *dict;
    ssize_t i;
    t_bool res;
    t_data *pair_data;

    dict = new(_dict);
    i = -1;
    while (++i < ac)
        ((Dict *)dict)->push(dict, (unsigned char *)av[i], &i, TYPE_INT);
    dict->dump(dict, "Dict dump1", typed_basic_print, "");
    res = ((Dict *)dict)->remove(dict, (unsigned char *)"hello");
    printf("RES REMOVE hello: [%s]\n", res == TRUE ? "TRUE" : "FALSE");
    dict->dump(dict, "Dict dump2", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add1", "testinho", TYPE_CSTRING);
    dict->dump(dict, "Dict dump3", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add2", &i, TYPE_INT);
    dict->dump(dict, "Dict dump4", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add3", &i, TYPE_INT);
    dict->dump(dict, "Dict dump5", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add4", &i, TYPE_INT);
    dict->dump(dict, "Dict dump6", typed_basic_print, "");
    ((Dict *)dict)->push(dict, (unsigned char *)"add5", &i, TYPE_INT);
    dict->dump(dict, "Dict dump7", typed_basic_print, "");
    res = ((Dict *)dict)->remove(dict, (unsigned char *)"add5");
    printf("RES REMOVE add5: [%s]\n", res == TRUE ? "TRUE" : "FALSE");
    dict->dump(dict, "Dict dump8", typed_basic_print, "");
    pair_data = ((Dict *)dict)->get_by_key(dict, (unsigned char *)"add1");
    printf("test get_key: type [%d] data [%s]\n", pair_data->type, pair_data->data);
    delete(dict);
    //printf("%d\n", ARG_LENGTH(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    Container   *ctn;
    t_data      **data_tab;

    data_tab = cstrings_to_data_array((char *[]){"foo", "bar", NULL});
    ctn = new(_spl_clist, data_tab, COPY_ALL, 0);
    ctn->insert_at(ctn, "totobite", TYPE_CSTRING, 666);
    ctn->dump(ctn, "list list", typed_basic_print, "");
    delete(ctn);
    free_typed_array(data_tab);

    String      *str;
    Container   *array;

    str = new(_string, "foobar", COPY_ALL, 0);
    array = str->split(str, _array, "o");
    printf("array size [%zd]\n", array->contained_size);
    printf("array[0] [%s]\n", ((t_data **)array->contained)[0]->data);
    printf("array[1] [%s]\n", ((t_data **)array->contained)[1]->data);
    delete(array);
    delete(str);
    return (1);
}
