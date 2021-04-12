#ifndef LISTS_H_
# define LISTS_H_

# include "containers.h"

typedef struct          s_list_data
{
    void                *data;
    struct s_list_data  *prev;
    struct s_list_data  *next;
} t_list_data;

typedef t_list_data *(*t_get_node)(const Object *list, ssize_t pos);
t_list_data         *get_nth_node(const Object *list, ssize_t pos);

typedef struct
{
    Container   base;
    t_get_node  nth_node;
} List;

typedef List SplList;
typedef List SplClist;
typedef List DblList;
typedef List DblClist;

typedef enum    e_list_type
{
    SIMPLE = 0,
    CIRC_SIMPLE,
    DOUBLE,
    CIRC_DOUBLE
} t_list_type;

// Constructors declarations
variadic_func_declare(Object *, spl_list_ctor, Object *class; t_data **to_copy; ssize_t copy_amount;)
#define _spl_list_ctor_(...)    call_variadic_func_wrapper(spl_list_ctor, __VA_ARGS__)

variadic_func_declare(Object *, spl_clist_ctor, Object *class; t_data **to_copy; ssize_t copy_amount;)
#define _spl_clist_ctor_(...)    call_variadic_func_wrapper(spl_clist_ctor, __VA_ARGS__)

variadic_func_declare(Object *, dbl_list_ctor, Object *class; t_data **to_copy; ssize_t copy_amount;)
#define _dbl_list_ctor_(...)    call_variadic_func_wrapper(dbl_list_ctor, __VA_ARGS__)

variadic_func_declare(Object *, dbl_clist_ctor, Object *class; t_data **to_copy; ssize_t copy_amount;)
#define _dbl_clist_ctor_(...)    call_variadic_func_wrapper(dbl_clist_ctor, __VA_ARGS__)

t_bool  _spl_list_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool  _spl_clist_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool  _dbl_list_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool  _dbl_clist_add(Object *list, void *data, t_type type, ssize_t pos);

t_bool  _spl_list_del(Object *list, ssize_t pos);
t_bool  _spl_clist_del(Object *list, ssize_t pos);
t_bool  _dbl_list_del(Object *list, ssize_t pos);
t_bool  _dbl_clist_del(Object *list, ssize_t pos);

t_bool  _list_erase(Object *self);

Object  *_list_front(const Object *list);
Object  *_list_end(const Object *list);
Object  *_list_at(const Object *list, ssize_t pos);

#define SPL_LIST    _spl_list
#define SPL_CLIST   _spl_clist
#define DBL_LIST    _dbl_list
#define DBL_CLIST   _dbl_clist

extern Class *SPL_LIST;
extern Class *SPL_CLIST;
extern Class *DBL_LIST;
extern Class *DBL_CLIST;

#endif /* !LISTS_H_ */
