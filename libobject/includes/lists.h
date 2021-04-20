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

// Class and constructors declarations
#define List_fields             \
        Container_fields        \
        t_get_node  nth_node;
#define List_definition                 \
        Container_definition,           \
        .erase = list_erase,           	\
        .front = list_front,           	\
        .back = list_end,              	\
        .at = list_at,                 	\
        .nth_node = get_nth_node
class_declaration(List)

#define SplList_fields  \
        List_fields
#define SplList_definition              \
        List_definition,                \
        .insert_at = spl_list_add,     	\
        .delete_at = spl_list_del
class_declaration(SplList)

#define SplClist_fields \
        List_fields
#define SplClist_definition             \
        List_definition,                \
        .insert_at = spl_clist_add,    	\
        .delete_at = spl_clist_del
class_declaration(SplClist)

#define DblList_fields  \
        List_fields
#define DblList_definition              \
        List_definition,                \
        .insert_at = dbl_list_add,     	\
        .delete_at = dbl_list_del
class_declaration(DblList)

#define DblClist_fields \
        List_fields
#define DblClist_definition             \
        List_definition,                \
        .insert_at = dbl_clist_add,    	\
        .delete_at = dbl_clist_del
class_declaration(DblClist)

ctor_declaration(Object *, SplList, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, SplClist, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, DblList, Object *class; t_data **to_copy; ssize_t copy_amount;)
ctor_declaration(Object *, DblClist, Object *class; t_data **to_copy; ssize_t copy_amount;)

t_bool  spl_list_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool  spl_clist_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool	dbl_list_add(Object *list, void *data, t_type type, ssize_t pos);
t_bool  dbl_clist_add(Object *list, void *data, t_type type, ssize_t pos);

t_bool  spl_list_del(Object *list, ssize_t pos);
t_bool  spl_clist_del(Object *list, ssize_t pos);
t_bool  dbl_list_del(Object *list, ssize_t pos);
t_bool  dbl_clist_del(Object *list, ssize_t pos);

t_bool  list_erase(Object *self);

Object  *list_front(const Object *list);
Object  *list_end(const Object *list);
Object  *list_at(const Object *list, ssize_t pos);

#endif /* !LISTS_H_ */
