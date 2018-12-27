#ifndef STRINGS_H_
# define STRINGS_H_

# include "containers.h"

typedef Object	*(*t_dup)(const Object *self);
Object		*_string_dup(const Object *self);

typedef size_t	(*t_str_findstr)(const Object *self, const char *substr);
size_t		_string_findstr(const Object *self, const char *substr);

typedef size_t	(*t_str_find)(const Object *self, int c);
size_t		_string_find(const Object *self, int c);
size_t		_string_lfind(const Object *self, int c);

typedef t_bool	(*t_str_match)(const Object *self, const char *compare);
t_bool		_string_match(const Object *self, const char *compare);

typedef int	(*t_str_nmatch)(const Object *self, const char *compare);
int		_string_nmatch(const Object *self, const char *compare);

typedef Object	*(*t_str_split)(const Object *self, const Class *type, const char *sep);
Object		*_string_split(const Object *self, const Class *type, const char *sep);

typedef struct {
  Container	base;
  t_dup		dup;
  t_str_findstr	find_str;
  t_str_find	find;
  t_str_find	lfind;
  t_str_match	match;
  t_str_nmatch	nmatch;
  t_str_split	split;
} String;

t_bool	_string_insert_at(Object *self, void *data, int pos);
t_bool	_string_delete_at(Object *self, int pos);
t_bool	_string_erase(Object *self);
void	_string_affect(Object *self, void *data);
Object	*_string_front(const Object *self);
Object	*_string_back(const Object *self);
Object	*_string_at(const Object *self, size_t pos);
void	_string_print(const Object *self, const char *title,
		      void (*f)(size_t i, const Object *elem, const char *prefix),
		      const char *prefix);
void	_string_basic_print(size_t i, const Object *elem, const char *prefix);

extern Class	*_string;

#endif /* !STRINGS_H_ */
