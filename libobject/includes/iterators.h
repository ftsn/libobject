#ifndef ITERATORS_H_
# define ITERATORS_H_

# include "containers.h"

typedef void	(*t_it_access)(Object *self);
void		_incr(Object *self);
void		_decr(Object *self);

typedef void	(*t_it_rand_access)(Object *self, int pos);
void		_jump(Object *self, int pos);

typedef Object	*(*t_rvalue)(Object *self);
Object		*_rvalue(Object *self);

typedef Object	*(*t_access_val)(Object *self, int pos);
Object		*_jmp_rvalue(Object *self, int pos);

typedef struct {
  Container		container;
  t_it_access		incr;
  t_it_access		decr;
  t_it_rand_access	jump;
  t_rvalue		rvalue;
  t_access_val		jmp_rvalue;
  int			pos;
} Iterator;

extern Class	*_it;

#endif /* !ITERATORS_H_ */
