#include <stdlib.h>
#include <stdio.h>
#include "lists.h"

static void	go_to_pos_in_list(t_list_data **list, t_list_data **tmp,
				  int pos)
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

static void	list_link(t_list_data **list, t_list_data **tmp,
			  t_list_data *elem, t_list_type type)
{
  if (type == CIRC_DOUBLE)
    elem->prev = (*tmp ? *tmp : (*list ? _list_end(*list) : elem));
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
  else if (!*tmp && (type == CIRC_SIMPLE || type == CIRC_DOUBLE))
    ((t_list_data *)_list_end(*list))->next = elem;
  if ((type == DOUBLE || type == CIRC_DOUBLE) && *list)
    (*list)->prev = elem;
}

static t_bool		list_add(t_list_data **list, void *data, int pos,
				 t_list_type type)
{
  t_list_data		*begin;
  t_list_data		*tmp;
  t_list_data		*elem;

  begin = *list;
  tmp = NULL;
  if (!(elem = malloc(sizeof(List))))
    return (FALSE);
  elem->data = data;
  go_to_pos_in_list(list, &tmp, pos);
  list_link(list, &tmp, elem, type);
  if (begin && pos > 0)
    *list = begin;
  if (pos == 0)
    *list = elem;
  return (TRUE);
}

t_bool	_spl_list_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained,
	       data, pos, SIMPLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_spl_clist_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained,
	       data, pos, CIRC_SIMPLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_dbl_list_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained,
	       data, pos, DOUBLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_dbl_clist_add(Object *list, void *data, int pos)
{
  if (list_add((t_list_data **)&((Container *)list)->contained,
	       data, pos, CIRC_DOUBLE) == TRUE)
    {
      ++((Container *)list)->contained_size;
      return (TRUE);
    }
  return (FALSE);
}

t_bool	_spl_list_del(Object *list, int pos)
{
  (void)list;
  (void)pos;
  return (TRUE);
}

t_bool	_spl_clist_del(Object *list, int pos)
{
  (void)list;
  (void)pos;
  return (TRUE);
}

t_bool	_dbl_list_del(Object *list, int pos)
{
  (void)list;
  (void)pos;
  return (TRUE);
}

t_bool	_dbl_clist_del(Object *list, int pos)
{
  (void)list;
  (void)pos;
  return (TRUE);
}

Object	*_list_front(Object *list)
{
  if (list && ((Container *)list)->empty(list) == FALSE)
    return (((t_list_data *)((Container *)list)->contained)->data);
  return (NULL);
}

Object		*_list_end(Object *list)
{
  t_list_data	*begin;
  t_list_data	*res;

  list = (t_list_data *)((Container *)list)->contained;
  res = begin = list;
  if (res && res->next)
    res = res->next;
  while (res && res->next && res->next != begin)
    res = res->next;
  list = begin;
  return (res ? res->data : NULL);
}

Object		*_list_at(Object *list, size_t pos)
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

void		_list_basic_print(size_t i, Object *elem, const char *prefix)
{
  t_list_data	*list;

  list = elem;
  printf("%s%u)prev [%s] cur [%s] next [%s]\n",
	 prefix, (unsigned int)i,
	 (list->prev ? (char *)list->prev->data : "null"),
	 (char *)list->data,
	 (list->next ? (char *)list->next->data : "null"));
}

void		_list_print(Object *container, const char *title,
			    void (*f)(size_t i, Object *elem,
				      const char *prefix),
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
