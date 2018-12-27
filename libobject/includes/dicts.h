#ifndef DICTS_H_
# define DICTS_H_

# include "containers.h"

typedef struct	s_pair {
  char		*key;
  void		*data;
}		t_pair;

typedef Object	*(*t_obj_by_key)(const Object *dict, const char *key);
Object		*_get_obj_by_key(const Object *dict, const char *key);

typedef t_bool	(*t_map_push)(Object *self, char *key, void *data);
t_bool		_dict_push_back(Object *self, char *key, void *data);

typedef struct {
  Container	base;
  t_obj_by_key	get_by_key;
  t_map_push	push_back;
} Dict;

void    pair_basic_print(size_t i, const Object *elem, const char *prefix);
void	_dict_print(const Object *container, const char *title,
		    void (*f)(size_t i, const Object *elem, const char *prefix),
		    const char *prefix);

extern Class	*_dict;

#endif /* !DICTS_H_ */
