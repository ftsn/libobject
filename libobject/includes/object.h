#ifndef OBJECT_H_
# define OBJECT_H_

#include <sys/types.h>
#include <stdarg.h>
#include "types.h"
#include "variadic.h"

#define BLUEPRINT_SUFFIX    _blueprint
#define CTOR_SUFFIX         _ctor
#define DEFINITION_SUFFIX   _definition
#define INIT_SUFFIX         _init

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
#define forward_declared_class_declaration(class_type)      \
        struct CAT(s_,class_type) {                         \
            Class;                                          \
            CAT(class_type,_fields)                         \
        };                                                  \
        extern Class    *CAT(class_type,BLUEPRINT_SUFFIX);

#define class_declaration(class_type)                       \
        typedef struct                                      \
        {                                                   \
            Class;                                          \
            CAT(class_type,_fields)                         \
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

#define class_definition(class_type, type, dtor)                                                \
        static Object   *CAT(_shallow_,CAT(class_type,CTOR_SUFFIX()))                           \
        {                                                                                       \
            return (new_obj(class_type));                                                       \
        }                                                                                       \
        _Pragma("GCC diagnostic push")                                                          \
        _Pragma("GCC diagnostic ignored \"-Woverride-init\"")                                   \
        static class_type CAT(class_type,INIT_SUFFIX) = {                                       \
            class_metadata(class_type, type, dtor),                                             \
            CAT(class_type,DEFINITION_SUFFIX)                                                   \
        };                                                                                      \
        _Pragma("GCC diagnostic pop")                                                           \
        Class * CAT(class_type,BLUEPRINT_SUFFIX) = (Class *)&CAT(class_type,INIT_SUFFIX);

#endif /* !OBJECT_H_ */
