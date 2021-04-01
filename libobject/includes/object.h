#ifndef OBJECT_H_
# define OBJECT_H_

# include <sys/types.h>
# include <stdarg.h>
# include "types.h"

typedef t_bool  (*ctor_t)(Object *self, va_list *args);
typedef void    (*dtor_t)(Object *self, va_list *args);

typedef struct
{
  const t_type  __type__;
  const size_t  __size__;
  ctor_t        __init__;
  dtor_t        __del__;
} Class;

t_bool  static_new(const Class *class, Class *dest, ...);
void    static_delete(Object *ptr, ...);
Object  *new(const Class *class, ...);
void    delete(Object *ptr, ...);

#endif /* !OBJECT_H_ */
