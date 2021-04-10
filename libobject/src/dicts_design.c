#include <stdlib.h>
#include "dicts.h"
#include "arrays.h"
#include "iterators.h"

static t_bool   _dict_ctor(Object *self, va_list *args)
{
    Container   *dict;

    (void)args;
    dict = self;
    if (dict_alloc(dict, DICT_ALLOC_SIZE(dict->contained_size)) == FALSE)
        return (FALSE);
    return (TRUE);
}

static void     _dict_dtor(Object *self, va_list *args)
{
    ssize_t     i;
    Container   **contained;
    //Iterator    *it;

    i = 0;
    contained = ((Container *)self)->contained;
    while (i < ((Dict *)self)->total_size)
    {
        if (contained[i])
        {
            delete(contained[i]);
            /*
            it = ((Container *)contained[i])->begin(contained[i]);
            if (it == NULL)
                return ;
            while (!it->reached_the_end)
            {
                free(((t_data *)it->dereference(it))->data);
                it->next(it);
            }
            delete(it);
            delete(contained[i]);
            */
        }
        ++i;
    }
    free(((Container *)self)->contained);
    ((Container *)self)->contained = NULL;
    ((Container *)self)->contained_size = 0;
    (void)args;
}

static Dict _dict_descr =
    {
        {
            {
                TYPE_DICT,
                sizeof(Dict),
                &_dict_ctor,
                &_dict_dtor
            },
            NULL,
            0,

            &_container_data,
            &_container_size,
            &_container_empty,

            NULL, // insert_at
            NULL, // push_back
            NULL, // delete_at
            NULL, // erase
            NULL,

            NULL, // front
            NULL, // back
            NULL, // at

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_end
        },
        0,
        &_get_obj_by_key,
        &_dict_push,
        &_dict_remove,
    };

Class   *_dict = (Class *)&_dict_descr;
