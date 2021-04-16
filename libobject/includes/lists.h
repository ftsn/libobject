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

typedef enum    e_list_type
{
    SIMPLE = 0,
    CIRC_SIMPLE,
    DOUBLE,
    CIRC_DOUBLE
} t_list_type;

#define LIST        _list
#define SPL_LIST    _spl_list
#define SPL_CLIST   _spl_clist
#define DBL_LIST    _dbl_list
#define DBL_CLIST   _dbl_clist

// Class and constructors declarations
class_declaration(List, LIST,
    Container   base;
    t_get_node  nth_node;
)
class_declaration(SplList, SPL_LIST,
    Container   base;
    t_get_node  nth_node;
)
class_declaration(SplClist, SPL_CLIST,
    Container   base;
    t_get_node  nth_node;
)
class_declaration(DblList, DBL_LIST,
    Container   base;
    t_get_node  nth_node;
)
class_declaration(DblClist, DBL_CLIST,
    Container   base;
    t_get_node  nth_node;
)

ctor_declaration(Object *, SPL_LIST, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, SPL_CLIST, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, DBL_LIST, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, DBL_CLIST, Object *class; t_data **to_copy; ssize_t copy_amount;)

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

extern Class *SPL_LIST;
extern Class *SPL_CLIST;
extern Class *DBL_LIST;
extern Class *DBL_CLIST;

#endif /* !LISTS_H_ */
