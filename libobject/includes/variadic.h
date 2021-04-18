#ifndef _VARIADIC_H_
# define _VARIADIC_H_

#define CAT_(a,b)   a##b
#define CAT(a,b)    CAT_(a,b)

#define _variadic_func_wrapper_proto(type, name)    type CAT(_variadic_func_wrapper_,name)(CAT(_typed_va_args_,name) *args)
#define variadic_func_definition(name)              CAT(_variadic_func_wrapper_,name)(CAT(_typed_va_args_,name) *args)
#define variadic_func_declare(type, name, ...)  \
        typedef struct {                        \
            int _sentinel;                      \
            __VA_ARGS__                         \
        } CAT(_typed_va_args_,name);                \
        _variadic_func_wrapper_proto(type, name);

#define variadic_var(name, value)               name = args->name ? args->name : (value);
#define call_variadic_func_wrapper(name, ...)   CAT(_variadic_func_wrapper_,name)(&(CAT(_typed_va_args_,name)) { ._sentinel = 0, __VA_ARGS__ })

#endif /* _VARIADIC_H_ */