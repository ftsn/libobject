#ifndef ARRAYS_H_
# define ARRAYS_H_

# include "containers.h"

# define COPY_ALL	0

typedef struct {
  Container	base;
} Array;

t_bool	_array_insert_at(Object *self, void *data, int pos);
t_bool	_array_delete_at(Object *self, int pos);
t_bool	_array_erase(Object *self);

Object	*_array_front(Object *self);
Object	*_array_back(Object *self);
Object	*_array_at(Object *self, size_t pos);

void	_array_print(Object *self, const char *title,
		     void (*f)(size_t i, Object *elem, const char *prefix),
		     const char *prefix);
void	array_basic_print(size_t i, Object *elem, const char *prefix);

extern Class	*_array;

#endif /* !ARRAYS_H_ */
