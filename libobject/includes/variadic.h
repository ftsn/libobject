#ifndef _VARIADIC_H_
# define _VARIADIC_H_

#define _variadic_func_wrapper_proto(type, name)    type _variadic_func_wrapper_##name(_typed_va_args_##name *args)
#define variadic_func_definition(name)              _variadic_func_wrapper_##name(_typed_va_args_##name *args)
#define variadic_func_declare(type, name, ...)  \
        typedef struct {                        \
            int _sentinel;                      \
            __VA_ARGS__                         \
        } _typed_va_args_##name;                \
        _variadic_func_wrapper_proto(type, name);

#define variadic_var(name, value)               name = args->name ? args->name : (value);
#define call_variadic_func_wrapper(name, ...)   _variadic_func_wrapper_##name(&(_typed_va_args_##name) { ._sentinel = 0, __VA_ARGS__ })

#endif /* _VARIADIC_H_ */