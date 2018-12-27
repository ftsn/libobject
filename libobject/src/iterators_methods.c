#include <stddef.h>
#include "iterators.h"

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

void	_ra_jump(Object *self, int pos)
{
  ((Iterator *)self)->pos += pos;
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
  if (it->pos >= 0 && it->pos < (int)it->container.contained_size)
    return ((it->container.at((Container *)it, it->pos)));
  return (NULL);
}
