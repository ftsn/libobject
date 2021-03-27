#include <stdlib.h>
#include "lists.h"

static t_bool   copy_ctor(Container *list, void **copy, ssize_t size)
{
    ssize_t     i;

    i = 0;
    if (size == COPY_ALL)
    {
        size = 0;
        while (copy[size])
            ++size;
    }
    while (i < size)
    {
        if (list->push_back(list, copy[i], TYPE_CHAR) == FALSE)
            return (FALSE);
        ++i;
    }
    return (TRUE);
}

static t_bool   _list_ctor(Object *self, va_list *args)
{
    Container   *list;
    ssize_t     nb_args;
    void        *copy;

    list = self;
    if ((copy = va_arg(*args, void *)))
        if (copy_ctor(list, copy, va_arg(*args, ssize_t)) == FALSE)
            return (FALSE);
    nb_args = va_arg(*args, ssize_t);
    while (nb_args > 0)
    {
        if (list->push_back(list, va_arg(*args, void *), va_arg(*args, t_type)) == FALSE)
            return (FALSE);
        --nb_args;
    }
    return (TRUE);
}

static void     _list_dtor(Object *self, va_list *args)
{
    Container   *list;

    list = self;
    while (list->contained)
    {
        if (list->delete_at(list, 0) == FALSE)
            return;
    }
    (void)args;
}

static Spl_list _spl_list_descr =
    {
        {
            {
                TYPE_LINKED_LIST,
                sizeof(List),
                &_list_ctor,
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
            NULL,
            NULL,

            &_list_front,
            &_list_end,
            &_list_at,

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_last
        },
        &get_nth_node
    };

static Spl_clist _spl_clist_descr =
    {
        {
            {
                TYPE_CIRCULAR_LINKED_LIST,
                sizeof(List),
                &_list_ctor,
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
            NULL,
            NULL,

            &_list_front,
            &_list_end,
            &_list_at,

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_last
        },
        &get_nth_node
    };

static Dbl_list _dbl_list_descr =
    {
        {
            {
                TYPE_DOUBLY_LINKED_LIST,
                sizeof(List),
                &_list_ctor,
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
            NULL,
            NULL,

            &_list_front,
            &_list_end,
            &_list_at,

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_last
        },
        &get_nth_node
    };

static Dbl_clist _dbl_clist_descr =
    {
        {
            {
                TYPE_CIRCULAR_DOUBLY_LINKED_LIST,
                sizeof(List),
                &_list_ctor,
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
            NULL,
            NULL,

            &_list_front,
            &_list_end,
            &_list_at,

            &_container_print,

            &_container_to_type,
            &_container_sub,
            &_container_map,

            &_container_begin,
            &_container_last
        },
        &get_nth_node
    };

Class *_spl_list = (Class *)&_spl_list_descr;
Class *_spl_clist = (Class *)&_spl_clist_descr;
Class *_dbl_list = (Class *)&_dbl_list_descr;
Class *_dbl_clist = (Class *)&_dbl_clist_descr;
