#include <stdlib.h>
#include "lists.h"
#include "iterators.h"

static Object   *_shared_list_ctor(Object *class, t_data **to_copy, ssize_t copy_amount)
{
    List        *list;

    list = class;
    if (!list)
        return (NULL);
    if (to_copy)
        if (ctn_copy_ctor((Container *)list, to_copy, copy_amount) == FALSE)
        {
            delete(list);
            return (NULL);
        }
    return (list);
}

Object          *ctor_definition(SplList)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

Object          *ctor_definition(SplClist)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

Object          *ctor_definition(DblList)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

Object          *ctor_definition(DblClist)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

static void     list_dtor(Object *self)
{
    Container   *list;

    list = self;
    list->erase(list);
    list->contained = NULL;
    list->contained_size = 0;
}

class_definition(SplList,
    {
        class_metadata(SplList, TYPE_LINKED_LIST, list_dtor),
        NULL,
        0,

        &_container_data,
        &_container_size,
        &_container_empty,

        &_spl_list_add,
        &_container_push_back,
        &_spl_list_del,
        &_list_erase,
        NULL,

        &_list_front,
        &_list_end,
        &_list_at,

        &_container_print,

        &_container_to_type,
        &_container_sub,
        &_container_map,

        &_container_begin,
        &_container_end
    },
    &get_nth_node      
)

class_definition(SplClist,
    {
        class_metadata(SplClist, TYPE_CIRCULAR_LINKED_LIST, list_dtor),
        NULL,
        0,

        &_container_data,
        &_container_size,
        &_container_empty,

        &_spl_clist_add,
        &_container_push_back,
        &_spl_clist_del,
        &_list_erase,
        NULL,

        &_list_front,
        &_list_end,
        &_list_at,

        &_container_print,

        &_container_to_type,
        &_container_sub,
        &_container_map,

        &_container_begin,
        &_container_end
    },
    &get_nth_node      
)

class_definition(DblList,
    {
        class_metadata(DblList, TYPE_DOUBLY_LINKED_LIST, list_dtor),
        NULL,
        0,

        &_container_data,
        &_container_size,
        &_container_empty,

        &_dbl_list_add,
        &_container_push_back,
        &_dbl_list_del,
        &_list_erase,
        NULL,

        &_list_front,
        &_list_end,
        &_list_at,

        &_container_print,

        &_container_to_type,
        &_container_sub,
        &_container_map,

        &_container_begin,
        &_container_end
    },
    &get_nth_node      
)

class_definition(DblClist,
    {
        class_metadata(DblClist, TYPE_CIRCULAR_DOUBLY_LINKED_LIST, list_dtor),
        NULL,
        0,

        &_container_data,
        &_container_size,
        &_container_empty,

        &_dbl_clist_add,
        &_container_push_back,
        &_dbl_clist_del,
        &_list_erase,
        NULL,

        &_list_front,
        &_list_end,
        &_list_at,

        &_container_print,

        &_container_to_type,
        &_container_sub,
        &_container_map,

        &_container_begin,
        &_container_end
    },
    &get_nth_node      
)