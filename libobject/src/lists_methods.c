#include <stdlib.h>
#include <stdio.h>
#include "lists.h"

static t_list_data  *get_last_node(t_list_data *list)
{
    t_list_data     *begin;

    begin = list;
    if (list && list->next)
        list = list->next;
    while (list && list->next && list->next != begin)
        list = list->next;
    return (list);
}

static void go_to_pos_in_list(t_list_data **list, t_list_data **tmp, ssize_t pos)
{
    ssize_t i;

    i = 0;
    *tmp = NULL;
    while (*list && i < pos)
    {
        ++i;
        *tmp = *list;
        *list = (*list)->next;
    }
}

static void list_link(t_list_data **list, t_list_data **tmp, t_list_data *end, t_list_data *elem, t_list_type type)
{
    if (type == CIRC_DOUBLE)
        elem->prev = (*tmp ? *tmp : (*list ? end : elem));
    else
        elem->prev = (type == DOUBLE ? *tmp : NULL);
    if (type == CIRC_SIMPLE || type == CIRC_DOUBLE)
        elem->next = (*list ? *list : elem);
    else
        elem->next = *list;
    if (!*list && !*tmp)
    {
        *list = elem;
        return;
    }
    if (*tmp)
        (*tmp)->next = elem;
    if (!*tmp && (type == CIRC_SIMPLE || type == CIRC_DOUBLE))
        end->next = elem;
    if ((type == DOUBLE || type == CIRC_DOUBLE) && *list)
        (*list)->prev = elem;
}

static t_bool   list_add(t_list_data **list, void *data, ssize_t pos, t_list_type type)
{
    t_list_data *begin;
    t_list_data *end;
    t_list_data *tmp;
    t_list_data *elem;

    begin = *list;
    end = get_last_node(begin);
    tmp = NULL;
    if (!(elem = malloc(sizeof(List))))
        return (FALSE);
    elem->data = data;
    go_to_pos_in_list(list, &tmp, pos);
    list_link(list, &tmp, end, elem, type);
    if (begin && pos > 0)
        *list = begin;
    if (pos == 0)
        *list = elem;
    return (TRUE);
}

static void     list_unlink(t_list_data **list, t_list_data **tmp, t_list_data *end, t_list_type type)
{
    t_list_data *next_node;

    if (!*list)
        return;
    next_node = (*list)->next;
    if (!*tmp)
    {
        if ((type == CIRC_SIMPLE || type == CIRC_DOUBLE))
            end->next = next_node;
        if (next_node)
            next_node->prev = type == CIRC_DOUBLE ? end : NULL;
    }
    else
    {
        (*tmp)->next = next_node;
        if (next_node)
            next_node->prev = (type == CIRC_DOUBLE || type == DOUBLE) ? *tmp : NULL;
    }
    free((*list)->data);
    free(*list);
    *list = NULL;
}

static t_bool   list_del(t_list_data **list, ssize_t pos, t_list_type type)
{
    t_list_data *begin;
    t_list_data *end;
    t_list_data *tmp;

    if (pos == 0)
    {
        if (*list)
            begin = ((*list)->next == *list || !(*list)) ? NULL : (*list)->next;
        else
            begin = NULL;
    }
    else
        begin = *list;
    end = get_last_node(*list);
    tmp = NULL;
    go_to_pos_in_list(list, &tmp, pos);
    list_unlink(list, &tmp, end, type);
    *list = begin;
    return (TRUE);
}

t_bool      spl_list_add(Object *self, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size)
        return (FALSE);
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&list->contained, typed_data, pos, SIMPLE) == TRUE)
    {
        ++list->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      spl_clist_add(Object *self, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size)
        return (FALSE);
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&list->contained, typed_data, pos, CIRC_SIMPLE) == TRUE)
    {
        ++list->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      dbl_list_add(Object *self, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size)
        return (FALSE);
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&list->contained, typed_data, pos, DOUBLE) == TRUE)
    {
        ++list->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      dbl_clist_add(Object *self, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size)
        return (FALSE);
    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&list->contained, typed_data, pos, CIRC_DOUBLE) == TRUE)
    {
        ++list->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      spl_list_del(Object *self, ssize_t pos)
{
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&list->contained, pos, SIMPLE);
    --list->contained_size;
    return (TRUE);
}

t_bool      spl_clist_del(Object *self, ssize_t pos)
{
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&list->contained, pos, CIRC_SIMPLE);
    --list->contained_size;
    return (TRUE);
}

t_bool      dbl_list_del(Object *self, ssize_t pos)
{
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&list->contained, pos, DOUBLE);
    --list->contained_size;
    return (TRUE);
}

t_bool      dbl_clist_del(Object *self, ssize_t pos)
{
    List    *list;

    list = self;
    if (pos < 0 || pos > list->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&list->contained, pos, CIRC_DOUBLE);
    --list->contained_size;
    return (TRUE);
}

Object  *list_front(const Object *list)
{
    return (((List *)list)->contained);
}

Object          *list_end(const Object *list)
{
    t_list_data *begin;
    t_list_data *res;

    res = begin = (t_list_data *)((List *)list)->contained;
    if (res && res->next)
        res = res->next;
    while (res && res->next && res->next != begin)
        res = res->next;
    return (res);
}

Object          *list_at(const Object *self, ssize_t pos)
{
    const List  *list = self;
    t_list_data *list_data;
    t_list_data *begin;
    ssize_t     i;

    if (pos < 0 || pos > list->contained_size)
        return (NULL);
    begin = list_data = (t_list_data *)list->contained;
    i = 0;
    if (list_data && i < pos)
    {
        list_data = list_data->next;
        ++i;
    }
    while (list_data && list_data->next && list_data->next != begin && i < pos)
    {
        list_data = list_data->next;
        ++i;
    }
    return (list_data);
}

t_bool      list_erase(Object *self)
{
    List    *list;

    list = self;
    while (list->contained)
        list->delete_at(list, 0);
    return (TRUE);
}

t_list_data     *get_nth_node(const Object *list, ssize_t pos)
{
    t_list_data *list_data;
    t_list_data *begin;
    ssize_t     i;

    begin = list_data = (t_list_data *)((const List *)list)->contained;
    i = 0;
    if (list_data && i < pos)
    {
        list_data = list_data->next;
        ++i;
    }
    while (list_data && list_data->next && list_data->next != begin && i < pos)
    {
        list_data = list_data->next;
        ++i;
    }
    return (list_data);
}
