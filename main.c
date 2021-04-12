#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"
#include "iterators.h"
#include "dicts.h"
#include "variadic.h"

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
    return ((t_data){TYPE_CSTRING, ((char *)data->data) + 1 });
}

int     main(int ac, char **av)
{
    (void)ac;
    (void)av;
    Container *array;
    array = new_obj(ARRAY, .to_copy = (t_data *[]){&(t_data){TYPE_CSTRING, "fabinho"}, &(t_data ){TYPE_CSTRING, "HELL0000"}, NULL}, .copy_amount = 1);
    array->dump(array, "Title smile", typed_basic_print, "");
    return (1);
}
