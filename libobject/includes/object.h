#ifndef OBJECT_H_
# define OBJECT_H_

#include <sys/types.h>
#include <stdarg.h>
#include "types.h"
#include "variadic.h"

typedef Object  *(*ctor_t)();
typedef void    (*dtor_t)(Object *self, va_list *args);

typedef struct
{
    const t_type __type__;
    const size_t __size__;
    ctor_t __init__;
    dtor_t __del__;
} Class;

t_bool      static_new(const Class *class, Class *dest, ...);
void        static_delete(Object *ptr, ...);
Object      *new (const Class *class, ...);
Object      *shallow_new_obj(const Class *class, ...);
void        delete (Object *ptr, ...);

Object      *_init_new_obj(const Class *class);

#define _new_obj(obj_type, ...)         obj_type##_ctor_(_init_new_obj(obj_type),##__VA_ARGS__)
#define new_obj(obj_type, ...)          \
        _new_obj(obj_type, __VA_ARGS__)

#define del_obj(obj, ...)  \
        _##obj##_dtor_(obj,##__VA_ARGS__)

#endif /* !OBJECT_H_ */
