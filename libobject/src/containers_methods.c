#include <string.h>
#include "containers.h"
#include "iterators.h"

Object	*_container_data(const Object *self)
{
  return (((Container *)self)->contained);
}

size_t	_container_size(const Object *container)
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
  return (self_c->insertAt(self_c, data, self_c->contained_size));
}

Object		*_container_to_type(Object *self, Class *type)
{
  Container	*self_c;
  Container	*container;
  size_t	i;

  self_c = self;
  container = new(type, NULL, 0);
  i = 0;
  while (i < self_c->contained_size)
    {
      container->push_back(container, self_c->at(self_c, i));
      ++i;
    }
  return (container);
}

Object		*_container_sub(Object *self, Class *type, int begin, int len)
{
  Container	*ctn;
  Container	*self_c;
  char		*at;
  int		i;

  i = 0;
  self_c = (Container *)self;
  if (self_c->contained_size > 0)
    {
      if (begin > 0 && begin >= (int)self_c->contained_size)
	begin = begin % self_c->contained_size;
      if (begin < 0)
	{
	  begin = -begin;
	  if (begin > (int)self_c->contained_size)
	    begin = begin % self_c->contained_size;
	  begin = self_c->contained_size - begin;
	}
    }
  else
    begin = 0;
  if (!(ctn = new(type, NULL, 0)))
    return (NULL);
  while (i < len && begin + i < (int)self_c->contained_size)
    {
      ctn->push_back(ctn, (at = (char *)self_c->at(self_c, begin + i)));
      ++i;
    }
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

Object		*_container_begin(Object *self)
{
  return (generate_it(self, BASIC));
}

Object	*_container_last(Object *self)
{
  return (generate_it(self, REVERSE));
}
