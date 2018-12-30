#ifndef ITERATORS_H_
# define ITERATORS_H_

# include "containers.h"

typedef enum {
  REVERSE = -1,
  BASIC
} t_it_type;

typedef void	(*t_it_access)(Object *self);
void		_ra_incr(Object *self);
void		_ra_decr(Object *self);
void		_list_incr(Object *self);
void		_list_decr(Object *self);

typedef void	(*t_it_rand_access)(Object *self, ssize_t pos);
void		_ra_jump(Object *self, ssize_t pos);
void		_list_jump(Object *self, ssize_t pos);

typedef Object	*(*t_rvalue)(Object *self);
Object		*_ra_rvalue(Object *self);
Object		*_list_rvalue(Object *self);

typedef Object	*(*t_access_val)(Object *self, ssize_t pos);
Object		*_ra_jmp_rvalue(Object *self, ssize_t pos);
Object		*_list_jmp_rvalue(Object *self, ssize_t pos);

typedef struct {
  Container		container;
  void			*cur;
  t_it_access		incr;
  t_it_access		decr;
  t_it_rand_access	jump;
  t_rvalue		rvalue;
  t_access_val		jmp_rvalue;
  int			pos;
} Iterator;

typedef Iterator	ArrayIt;
typedef Iterator	StringIt;
typedef Iterator	DictIt;
typedef Iterator	ListIt;

extern Class	*_array_it;
extern Class	*_string_it;
extern Class	*_dict_it;
extern Class	*_list_it;

#endif /* !ITERATORS_H_ */
