#ifndef LISTS_H_
# define LISTS_H_

# include "containers.h"

typedef struct		s_list_data
{
  void			*data;
  struct s_list_data	*prev;
  struct s_list_data	*next;
}			t_list_data;

typedef t_list_data	*(*t_get_node)(const Object *list, size_t pos);
t_list_data		*get_nth_node(const Object *list, size_t pos);

typedef struct {
  Container	base;
  t_get_node	nth_node;
} List;

typedef List	Spl_list;
typedef List	Spl_clist;
typedef List	Dbl_list;
typedef List	Dbl_clist;

typedef enum	e_list_type {
  SIMPLE = 0,
  CIRC_SIMPLE,
  DOUBLE,
  CIRC_DOUBLE
}		t_list_type;

t_bool	_spl_list_add(Object *list, void *data, int pos);
t_bool	_spl_clist_add(Object *list, void *data, int pos);
t_bool	_dbl_list_add(Object *list, void *data, int pos);
t_bool	_dbl_clist_add(Object *list, void *data, int pos);

t_bool	_spl_list_del(Object *list, int pos);
t_bool	_spl_clist_del(Object *list, int pos);
t_bool	_dbl_list_del(Object *list, int pos);
t_bool	_dbl_clist_del(Object *list, int pos);

Object	*_list_front(const Object *list);
Object	*_list_end(const Object *list);
Object	*_list_at(const Object *list, size_t pos);

void	_list_print(const Object *list, const char *title,
		    void (*f)(size_t i, const Object *elem, const char *prefix),
		    const char *prefix);
void	_list_basic_print(size_t i, const Object *elem, const char *prefix);

extern Class	*_spl_list;
extern Class	*_spl_clist;
extern Class	*_dbl_list;
extern Class	*_dbl_clist;

#endif /* !LISTS_H_ */
