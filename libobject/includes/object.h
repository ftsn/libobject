#ifndef OBJECT_H_
# define OBJECT_H_

#include <sys/types.h>
#include <stdarg.h>
#include "types.h"
#include "variadic.h"

typedef Object  *(*ctor_t)();
typedef void    (*dtor_t)(Object *self);

typedef struct
{
    const t_type __type__;
    const size_t __size__;
    ctor_t __init__;
    dtor_t __del__;
} Class;

Object      *shallow_new_obj(const Class *class, ...);
void        delete(Object *ptr);

Object      *_init_new_obj(const Class *class);

#define new_obj_(obj_type, ...)         call_variadic_func_wrapper(obj_type##_ctor, _init_new_obj(obj_type),##__VA_ARGS__)
#define new_obj(obj_type, ...)          new_obj_(obj_type,##__VA_ARGS__)

#define ctor_declaration_(type, name, ...)      variadic_func_declare(type, name##_ctor,##__VA_ARGS__)
#define ctor_declaration(type, name, ...)       ctor_declaration_(type, name,##__VA_ARGS__)

#define ctor_definition_(name)                  variadic_func_definition(name##_ctor)
#define ctor_definition(name)                   ctor_definition_(name)

#define class_declaration(class_type, blueprint_name, ...)  \
        typedef struct                                      \
        {                                                   \
            __VA_ARGS__                                     \
        } class_type;                                       \
        extern Class    *blueprint_name;
#define class_definition_(class_type, blueprint_name, ...)  \
        static Object   *_shallow_##blueprint_name##_ctor() \
        {                                                   \
            return (new_obj(blueprint_name));               \
        }                                                   \
        static class_type blueprint_name##_ = {             \
            __VA_ARGS__                                     \
        };                                                  \
        Class *blueprint_name = (Class *)&blueprint_name##_;
#define class_definition(class_type, blueprint_name, ...)   class_definition_(class_type, blueprint_name,##__VA_ARGS__)

#define TEST    _test
class_declaration(Test, TEST, Class base; int nb;)
ctor_declaration(Object *, TEST, Object *class; int nb;)

#endif /* !OBJECT_H_ */
