#include <stdlib.h>
#include "lists.h"
#include "iterators.h"

static Object   *_shared_list_ctor(Object *class, t_data **to_copy, ssize_t copy_amount)
{
    Container   *list;

    list = class;
    if (!list)
        return (NULL);
    if (to_copy)
        if (ctn_copy_ctor(list, to_copy, copy_amount) == FALSE)
        {
            delete(list);
            return (NULL);
        }
    return (list);
}

Object          *ctor_definition(SPL_LIST)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

static Object   *_shallow_spl_list_ctor()
{
    return (new_obj(SPL_LIST));
}

Object          *ctor_definition(SPL_CLIST)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

static Object   *_shallow_spl_clist_ctor()
{
    return (new_obj(SPL_CLIST));
}

Object          *ctor_definition(DBL_LIST)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

static Object   *_shallow_dbl_list_ctor()
{
    return (new_obj(DBL_LIST));
}

Object          *ctor_definition(DBL_CLIST)
{
    return (_shared_list_ctor(args->class, args->to_copy, args->copy_amount));
}

static Object   *_shallow_dbl_clist_ctor()
{
    return (new_obj(DBL_CLIST));
}

static void     _list_dtor(Object *self, va_list *args)
{
    Container   *list;

    list = self;
    list->erase(list);
    list->contained = NULL;
    list->contained_size = 0;
    (void)args; 
}

static SplList _spl_list_descr =
    {
        {
            {
                TYPE_LINKED_LIST,
                sizeof(List),
                &_shallow_spl_list_ctor,
                &_list_dtor
            },
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
    };

static SplClist _spl_clist_descr =
    {
        {
            {
                TYPE_CIRCULAR_LINKED_LIST,
                sizeof(List),
                &_shallow_spl_clist_ctor,
                &_list_dtor
            },
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
    };

static DblList _dbl_list_descr =
    {
        {
            {
                TYPE_DOUBLY_LINKED_LIST,
                sizeof(List),
                &_shallow_dbl_list_ctor,
                &_list_dtor
            },
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
    };

static DblClist _dbl_clist_descr =
    {
        {
            {
                TYPE_CIRCULAR_DOUBLY_LINKED_LIST,
                sizeof(List),
                &_shallow_dbl_clist_ctor,
                &_list_dtor
            },
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
    };

Class *SPL_LIST = (Class *)&_spl_list_descr;
Class *SPL_CLIST = (Class *)&_spl_clist_descr;
Class *DBL_LIST = (Class *)&_dbl_list_descr;
Class *DBL_CLIST = (Class *)&_dbl_clist_descr;
