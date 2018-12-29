#ifndef CONTAINERS_H_
# define CONTAINERS_H_

# include "object.h"

# define COPY_ALL	0

typedef Object	*(*t_data)(const Object *self);
Object		*_container_data(const Object *self);

typedef size_t	(*t_size)(const Object *self);
size_t		_container_size(const Object *self);

typedef t_bool	(*t_empty)(const Object *self);
t_bool		_container_empty(const Object *self);

typedef t_bool	(*t_rand_insert)(Object *self, void *data, int pos);

typedef t_bool	(*t_insert)(Object *self, void *data);
t_bool		_container_push_back(Object *self, void *data);

typedef t_bool	(*t_rand_delete)(Object *self, int pos);

typedef t_bool	(*t_delete)(Object *self);

typedef void	(*t_affect)(Object *self, void *data);

typedef Object	*(*t_access)(const Object *container);

typedef Object	*(*t_rand_access)(const Object *contained, size_t pos);

typedef void	(*t_dump)(const Object *self, const char *title,
			  void (*f)(size_t i, const Object *elem, const char *prefix),
			  const char *prefix);

typedef Object	*(*t_converter)(Object *self, Class *type);
Object		*_container_to_type(Object *self, Class *type);

typedef Object	*(*t_sub)(Object *self, Class *type, int begin, int len);
Object		*_container_sub(Object *self, Class *type, int begin, int len);

typedef Object	*(*t_it_create)(Object *self);
Object	       	*_container_begin(Object *self);
Object	       	*_container_last(Object *self);

typedef struct {
  Class		base;
  Object	*contained;

  size_t	contained_size;
  t_data	data;

  t_size	size;
  t_empty	empty;

  t_rand_insert	insertAt;
  t_insert	push_back;
  t_rand_delete	deleteAt;
  t_delete	erase;
  t_affect	affect;

  t_access	front;
  t_access	back;
  t_rand_access	at;

  t_dump	dump;

  t_converter	convert;
  t_sub		sub;

  t_it_create   first;
  t_it_create	last;
} Container;

#endif /* !CONTAINERS_H_ */
