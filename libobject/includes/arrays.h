#ifndef ARRAYS_H_
# define ARRAYS_H_

# include "containers.h"

typedef struct {
  Container	base;
} Array;

t_bool	_array_insert_at(Object *self, void *data, ssize_t pos);
t_bool	_array_delete_at(Object *self, ssize_t pos);
t_bool	_array_erase(Object *self);

Object	*_array_front(const Object *self);
Object	*_array_back(const Object *self);
Object	*_array_at(const Object *self, ssize_t pos);

void	array_basic_print(ssize_t i, const t_data *elem, const char *prefix);

extern Class	*_array;

#endif /* !ARRAYS_H_ */
