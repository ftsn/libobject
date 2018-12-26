#ifndef OBJECT_H_
# define OBJECT_H_

# include <sys/types.h>
# include <stdarg.h>
# include "utils.h"

typedef void Object;


typedef t_bool (*ctor_t)(Object *self, va_list *args);
typedef void (*dtor_t)(Object *self, va_list *args);

typedef struct {
  const size_t	__size__;
  const char	*__name__;
  ctor_t	__init__;
  dtor_t	__del__;
} Class;

Object	*new(Class *class, ...);
void	delete(Object **ptr, ...);

#endif /* !OBJECT_H_ */
