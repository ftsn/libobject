#include <stddef.h>
#include "iterators.h"
#include "lists.h"

/*
** Random access methods
*/
void		_ra_incr(Object *self)
{
  Iterator	*it;

  it = self;
  ++it->pos;
  it->cur = it->container.at((Container *)it, it->pos);
}

void		_ra_decr(Object *self)
{
  Iterator	*it;

  it = self;
  --it->pos;
  it->cur = it->container.at((Container *)it, it->pos);
}

void		_ra_jump(Object *self, int pos)
{
  Iterator	*it;

  it = self;
  it->pos += pos;
  it->cur = it->container.at((Container *)it, it->pos);
}

Object		*_ra_rvalue(Object *self)
{
  Iterator	*it;

  it = self;
  if (it->pos >= 0 && it->pos < (int)it->container.contained_size)
    return (it->cur);
  return (NULL);
}

Object		*_ra_jmp_rvalue(Object *self, int pos)
{
  Iterator	*it;

  it = self;
  it->jump(it, pos);
  return (it->rvalue(it));
}

/*
** List methods
*/
void		_list_incr(Object *self)
{
  Iterator	*it;

  it = self;
  ++it->pos;
  it->cur = ((t_list_data *)it->cur)->next;
}

void		_list_decr(Object *self)
{
  Iterator	*it;

  it = self;
  --it->pos;
  it->cur = ((t_list_data *)it->cur)->prev;
}

void		_list_jump(Object *self, int pos)
{
  Iterator	*it;
  int		i;

  i = 0;
  it = self;
  it->pos += pos;
  if (pos < 0)
    {
      while (i > pos)
	{
	  it->cur = ((t_list_data *)it->cur)->prev;
	  --i;
	}
    }
  else
    {
      while (i < pos)
	{
	  it->cur = ((t_list_data *)it->cur)->next;
	  ++i;
	}
    }
}

Object		*_list_rvalue(Object *self)
{
  Iterator	*it;

  it = self;
  if (it->pos >= 0 && it->pos < (int)it->container.contained_size)
    return (((t_list_data *)it->cur)->data);
  return (NULL);
}

Object		*_list_jmp_rvalue(Object *self, int pos)
{
  Iterator	*it;

  it = self;
  it->jump(it, pos);
  return (it->rvalue(it));
}
