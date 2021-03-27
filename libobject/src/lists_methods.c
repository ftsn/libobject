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

t_bool      _spl_list_add(Object *list, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;

    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&((Container *)list)->contained, typed_data, pos, SIMPLE) == TRUE)
    {
        ++((Container *)list)->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      _spl_clist_add(Object *list, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;

    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&((Container *)list)->contained, typed_data, pos, CIRC_SIMPLE) == TRUE)
    {
        ++((Container *)list)->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      _dbl_list_add(Object *list, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;

    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&((Container *)list)->contained, typed_data, pos, DOUBLE) == TRUE)
    {
        ++((Container *)list)->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool      _dbl_clist_add(Object *list, void *data, t_type type, ssize_t pos)
{
    t_data  *typed_data;

    if (!(typed_data = malloc(sizeof(t_data))))
        return (FALSE);
    typed_data->type = type;
    typed_data->data = data;
    if (list_add((t_list_data **)&((Container *)list)->contained, typed_data, pos, CIRC_DOUBLE) == TRUE)
    {
        ++((Container *)list)->contained_size;
        return (TRUE);
    }
    return (FALSE);
}

t_bool  _spl_list_del(Object *list, ssize_t pos)
{
    if (pos > ((Container *)list)->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&((Container *)list)->contained, pos, SIMPLE);
    --((Container *)list)->contained_size;
    return (TRUE);
}

t_bool  _spl_clist_del(Object *list, ssize_t pos)
{
    if (pos > ((Container *)list)->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&((Container *)list)->contained, pos, CIRC_SIMPLE);
    --((Container *)list)->contained_size;
    return (TRUE);
}

t_bool  _dbl_list_del(Object *list, ssize_t pos)
{
    if (pos > ((Container *)list)->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&((Container *)list)->contained, pos, DOUBLE);
    --((Container *)list)->contained_size;
    return (TRUE);
}

t_bool  _dbl_clist_del(Object *list, ssize_t pos)
{
    if (pos > ((Container *)list)->contained_size - 1)
        return (FALSE);
    list_del((t_list_data **)&((Container *)list)->contained, pos, CIRC_DOUBLE);
    --((Container *)list)->contained_size;
    return (TRUE);
}

Object  *_list_front(const Object *list)
{
    return (((Container *)list)->contained);
}

Object          *_list_end(const Object *list)
{
    t_list_data *begin;
    t_list_data *res;

    res = begin = (t_list_data *)((Container *)list)->contained;
    if (res && res->next)
        res = res->next;
    while (res && res->next && res->next != begin)
        res = res->next;
    return (res);
}

Object          *_list_at(const Object *list, ssize_t pos)
{
    t_list_data *list_data;
    t_list_data *begin;
    ssize_t     i;

    begin = list_data = (t_list_data *)((Container *)list)->contained;
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

void                    list_basic_print(ssize_t i, const t_data *elem, const char *prefix)
{
    const t_list_data   *list;

    list = (const t_list_data *)elem;
    printf("%s%d)prev [%s] cur [%s] next [%s]\n",
           prefix, (int)i,
           (list->prev ? (char *)list->prev->data : "null"),
           (char *)list->data,
           (list->next ? (char *)list->next->data : "null"));
}

void    _list_affect(Object *list, void *data)
{
    ((Container *)list)->contained = data;
}

t_list_data     *get_nth_node(const Object *list, ssize_t pos)
{
    t_list_data *list_data;
    t_list_data *begin;
    ssize_t     i;

    begin = list_data = (t_list_data *)((Container *)list)->contained;
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
