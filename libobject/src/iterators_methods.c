#include <stddef.h>
#include "iterators.h"

void	_incr(Object *self)
{
  ++((Iterator *)self)->pos;
}

void	_decr(Object *self)
{
  --((Iterator *)self)->pos;
}

void	_jump(Object *self, int pos)
{
  ((Iterator *)self)->pos += pos;
}

Object		*_rvalue(Object *self)
{
  Iterator	*it;

  it = self;
  if (it->pos >= 0 && it->pos < (int)it->container.contained_size)
    return ((it->container.at((Container *)it, it->pos)));
  return (NULL);
}

Object		*_jmp_rvalue(Object *self, int pos)
{
  Iterator	*it;

  it = self;
  it->jump(it, pos);
  if (it->pos >= 0 && it->pos < (int)it->container.contained_size)
    return ((it->container.at((Container *)it, it->pos)));
  return (NULL);
}
