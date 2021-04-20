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

class_definition(SplList, TYPE_LINKED_LIST, list_dtor)
class_definition(SplClist, TYPE_CIRCULAR_LINKED_LIST, list_dtor)
class_definition(DblList, TYPE_DOUBLY_LINKED_LIST, list_dtor)
class_definition(DblClist, TYPE_CIRCULAR_DOUBLY_LINKED_LIST, list_dtor)