#ifndef OBJECT_H_
# define OBJECT_H_

#include <sys/types.h>
#include <stdarg.h>
#include "types.h"
#include "variadic.h"

#define BLUEPRINT_SUFFIX    _blueprint
#define CTOR_SUFFIX         _ctor
#define DEFINITION_SUFFIX   _def

#define BLUEPRINT(type)     CAT(type,BLUEPRINT_SUFFIX)       

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

#define new_obj(class_type, ...)                call_variadic_func_wrapper(CAT(class_type,CTOR_SUFFIX), _init_new_obj(CAT(class_type,BLUEPRINT_SUFFIX)),##__VA_ARGS__)
#define ctor_declaration(type, class_type, ...) variadic_func_declare(type, CAT(class_type,CTOR_SUFFIX),##__VA_ARGS__)
#define ctor_definition(name)                   variadic_func_definition(CAT(name,CTOR_SUFFIX))

// Forward class declaration macros
#define forward_class_declaration(class_type)   typedef struct CAT(s_,class_type)   class_type;

// Class declaration macros
#define forward_declared_class_declaration(class_type, ...) \
        struct CAT(s_,class_type) {                         \
            __VA_ARGS__                                     \
        };                                                  \
        extern Class    *CAT(class_type,BLUEPRINT_SUFFIX);

#define class_declaration(class_type, ...)                  \
        typedef struct                                      \
        {                                                   \
            __VA_ARGS__                                     \
        } class_type;                                       \
        extern Class    *CAT(class_type,BLUEPRINT_SUFFIX);

// Class definition macros
#define class_metadata(class_type, type, dtor)          \
        {                                               \
            type,                                       \
            sizeof(class_type),                         \
            CAT(_shallow_,CAT(class_type,CTOR_SUFFIX)), \
            dtor                                        \
        }
#define class_definition(class_type, ...)                                                       \
        static Object   *CAT(_shallow_,CAT(class_type,CTOR_SUFFIX()))                           \
        {                                                                                       \
            return (new_obj(class_type));                                                       \
        }                                                                                       \
        static class_type CAT(class_type,DEFINITION_SUFFIX) = {                                 \
            __VA_ARGS__                                                                         \
        };                                                                                      \
        Class * CAT(class_type,BLUEPRINT_SUFFIX) = (Class *)&CAT(class_type,DEFINITION_SUFFIX);

class_declaration(Test, Class; int nb;)
ctor_declaration(Object *, Test, Object *class; int nb;)

#endif /* !OBJECT_H_ */
