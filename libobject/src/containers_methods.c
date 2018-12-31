#include <string.h>
#include "containers.h"
#include "iterators.h"

Object	*_container_data(const Object *self)
{
  return (((Container *)self)->contained);
}

ssize_t	_container_size(const Object *container)
{
  return (((Container *)container)->contained_size);
}

t_bool	_container_empty(const Object *container)
{
  return (((Container *)container)->contained_size == 0 ? TRUE : FALSE);
}

t_bool		_container_push_back(Object *self, void *data)
{
  Container	*self_c;

  self_c = self;
  return (self_c->insert_at(self_c, data, self_c->contained_size));
}

Object		*_container_to_type(Object *self, Class *type)
{
  Container	*self_c;
  Container	*container;
  ssize_t	i;

  self_c = self;
  container = new(type, NULL, 0);
  i = 0;
  while (i < self_c->contained_size)
    {
      if (container->push_back(container, self_c->at(self_c, i)) == FALSE)
	{
	  delete((void **)&container);
	  return (NULL);
	}
      ++i;
    }
  return (container);
}

Object		*_container_sub(Object *self, Class *type, ssize_t begin, ssize_t len)
{
  Container	*ctn;
  Container	*self_c;
  void		*at;
  ssize_t      	i;

  i = 0;
  self_c = (Container *)self;
  if (self_c->contained_size > 0)
    {
      if (begin > 0 && begin >= self_c->contained_size)
	begin = begin % self_c->contained_size;
      if (begin < 0)
	{
	  begin = -begin;
	  if (begin > self_c->contained_size)
	    begin = begin % self_c->contained_size;
	  begin = self_c->contained_size - begin;
	}
    }
  else
    begin = 0;
  if (!(ctn = new(type, NULL, 0)))
    return (NULL);
  while (i < len && begin + i < self_c->contained_size)
    {
      if (ctn->push_back(ctn, (at = self_c->at(self_c, begin + i))) == FALSE)
	{
	  delete((void **)&ctn);
	  return (NULL);
	}
      ++i;
    }
  return (ctn);
}

Object		*_container_map(Object *self, Class *type, void *(*fptr)(ssize_t i, void *cur))
{
  Container	*ctn;
  Iterator	*it;
  ssize_t	i;

  if (!(ctn = new(type, NULL, 0)))
    return (NULL);
  if (!(it = ((Container *)self)->first(self)))
    {
      delete((void **)&ctn);
      return (NULL);
    }
  i = 0;
  while (it->rvalue(it) != NULL)
    {
      if (ctn->push_back(ctn, fptr(i, it->rvalue(it))) == FALSE)
	{
	  delete((void **)&it);
	  delete((void **)&ctn);
	  return (NULL);
	}
      ++i;
      it->incr(it);
    }
  delete((void **)&it);
  return (ctn);
}

static Object	*generate_it(Object *self, t_it_type type)
{
  Iterator	*it;
  Container	*ctn;

  ctn = self;
  it = NULL;
  if (!strcmp(ctn->base.__name__, "linked List") ||
      !strcmp(ctn->base.__name__, "Circular linked List") ||
      !strcmp(ctn->base.__name__, "Doubly linked List") ||
      !strcmp(ctn->base.__name__, "Circular Doubly linked List"))
    it = new(_list_it, self, type);
  if (!strcmp(ctn->base.__name__, "Array"))
    it = new(_array_it, self, type);
  if (!strcmp(ctn->base.__name__, "Dict"))
    it = new(_dict_it, self, type);
  if (!strcmp(ctn->base.__name__, "String"))
    it = new(_string_it, self, type);
  return (it);
}

Object 	*_container_begin(Object *self)
{
  return (generate_it(self, BASIC));
}

Object	*_container_last(Object *self)
{
  return (generate_it(self, REVERSE));
}
