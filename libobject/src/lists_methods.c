#include <stdlib.h>
#include <stdio.h>
#include "lists.h"

static t_list_data     	*get_last_node(t_list_data *list)
{
  t_list_data	*begin;

  begin = list;
  if (list && list->next)
    list = list->next;
  while (list && list->next && list->next != begin)
    list = list->next;
  return (list);
}

static void	go_to_pos_in_list(t_list_data **list, t_list_data **tmp, int pos)
{
  int		i;

  i = 0;
  *tmp = NULL;
  while (*list && i < pos)
    {
      ++i;
      *tmp = *list;
      *list = (*list)->next;
    }
}

static void	list_link(t_list_data **list, t_list_data **tmp, t_list_data *end, t_list_data *elem, t_list_type type)
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
      return ;
    }
  if (*tmp)
    (*tmp)->next = elem;
  if (!*tmp && (type == CIRC_SIMPLE || type == CIRC_DOUBLE))
    end->next = elem;
  if ((type == DOUBLE || type == CIRC_DOUBLE) && *list)
    (*list)->prev = elem;
}

static t_bool  	list_add(t_list_data **list, void *data, int pos, t_list_type type)
{
  t_list_data  	*begin;
  t_list_data  	*end;
  t_list_data  	*tmp;
  t_list_data  	*elem;

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

static void	list_unlink(t_list_data **list, t_list_data **tmp, t_list_data *end, t_list_type type)
{
  t_list_data	*next_node;

  if (!*list)
    return ;
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

static t_bool		list_del(t_list_data **list, int pos, t_list_type type)
{
  t_list_data		*begin;
  t_list_data		*end;
  t_list_data		*tmp;

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

t_bool	_spl_list_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained, data, pos, SIMPLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_spl_clist_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained, data, pos, CIRC_SIMPLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_dbl_list_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained, data, pos, DOUBLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_dbl_clist_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained, data, pos, CIRC_DOUBLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_spl_list_del(Object *list, int pos)
{
  if (pos > (int)((Container *)list)->contained_size - 1)
    return (FALSE);
  if (list_del((t_list_data **)&((Container *)list)->contained, pos, SIMPLE) == TRUE)
    --((Container *)list)->contained_size;
  return (TRUE);
}

t_bool	_spl_clist_del(Object *list, int pos)
{
  if (pos > (int)((Container *)list)->contained_size - 1)
    return (FALSE);
  if (list_del((t_list_data **)&((Container *)list)->contained, pos, CIRC_SIMPLE) == TRUE)
    --((Container *)list)->contained_size;
  return (TRUE);
}

t_bool	_dbl_list_del(Object *list, int pos)
{
  if (pos > (int)((Container *)list)->contained_size - 1)
    return (FALSE);
  if (list_del((t_list_data **)&((Container *)list)->contained, pos, DOUBLE) == TRUE)
    --((Container *)list)->contained_size;
  return (TRUE);
}

t_bool	_dbl_clist_del(Object *list, int pos)
{
  if (pos > (int)((Container *)list)->contained_size - 1)
    return (FALSE);
  if (list_del((t_list_data **)&((Container *)list)->contained, pos, CIRC_DOUBLE) == TRUE)
    --((Container *)list)->contained_size;
  return (TRUE);
}

Object	*_list_front(const Object *list)
{
  if (list && ((Container *)list)->empty(list) == FALSE)
    return (((t_list_data *)((Container *)list)->contained)->data);
  return (NULL);
}

Object		*_list_end(const Object *list)
{
  t_list_data	*begin;
  t_list_data	*res;

  res = begin = (t_list_data *)((Container *)list)->contained;
  if (res && res->next)
    res = res->next;
  while (res && res->next && res->next != begin)
    res = res->next;
  (void)list;
  return (res ? res->data : NULL);
}

Object		*_list_at(const Object *list, size_t pos)
{
  t_list_data	*list_data;
  t_list_data	*begin;
  size_t	i;

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
  return (list_data ? list_data->data : NULL);
}

void			_list_basic_print(size_t i, const Object *elem, const char *prefix)
{
  const t_list_data	*list;

  list = elem;
  printf("%s%u)prev [%s] cur [%s] next [%s]\n",
	 prefix, (unsigned int)i,
	 (list->prev ? (char *)list->prev->data : "null"),
	 (char *)list->data,
	 (list->next ? (char *)list->next->data : "null"));
}

void		_list_print(const Object *container, const char *title,
			    void (*f)(size_t i, const Object *elem, const char *prefix),
			    const char *prefix)
{
  t_list_data	*begin;
  t_list_data	*list;
  char		*concat_prefix;
  size_t	i;

  list = (t_list_data *)((Container *)container)->contained;
  if (!(concat_prefix = concat(prefix, "  ")))
    return ;
  if (title)
    printf("%s%s\n", prefix, title);
  begin = list;
  i = 0;
  do
    {
      if (list)
	{
	  f(i, list, concat_prefix);
	  list = list->next;
	  ++i;
	}
    }
  while (list && list != begin);
  list = begin;
  free(concat_prefix);
}

void	_list_affect(Object *list, void *data)
{
  ((Container *)list)->contained = data;
}

t_list_data	*get_nth_node(const Object *list, size_t pos)
{
  t_list_data	*list_data;
  t_list_data	*begin;
  size_t	i;

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
