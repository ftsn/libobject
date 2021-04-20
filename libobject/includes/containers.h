#ifndef CONTAINERS_H_
# define CONTAINERS_H_

# include "object.h"

# define COPY_ALL   ((ssize_t)-1)

typedef Object  *(*t_get_data)(const Object *self);
Object          *container_data(const Object *self);

typedef ssize_t (*t_size)(const Object *self);
ssize_t         container_size(const Object *self);

typedef t_bool  (*t_empty)(const Object *self);
t_bool          container_empty(const Object *self);

typedef t_bool  (*t_rand_insert)(Object *self, void *data, t_type type, ssize_t pos);
t_bool          container_insert_at(Object *self, void *data, t_type type, ssize_t pos);

typedef t_bool  (*t_insert)(Object *self, void *data, t_type type);
t_bool          container_push_back(Object *self, void *data, t_type type);

typedef t_bool  (*t_rand_delete)(Object *self, ssize_t pos);

typedef t_bool  (*t_delete)(Object *self);

typedef Object  *(*t_access)(const Object *container);

typedef Object  *(*t_rand_access)(const Object *contained, ssize_t pos);

typedef void    (*t_dump)(Object *self, const char *title, void (*f)(const t_data *elem, const char *prefix), const char *prefix);
void            container_print(Object *container, const char *title, void (*f)(const t_data *elem, const char *prefix), const char *prefix);
void            typed_basic_print(const t_data *elem, const char *prefix);

typedef Object  *(*t_converter)(Object *self, Class *type);
Object          *container_to_type(Object *self, Class *type);

typedef Object  *(*t_sub)(Object *self, Class *type, ssize_t begin, ssize_t len);
Object          *container_sub(Object *self, Class *type, ssize_t begin, ssize_t len);

typedef Object  *(*t_map)(Object *self, Class *type, t_data (*fptr)(ssize_t i, void *cur));
Object          *container_map(Object *self, Class *type, t_data (*fptr)(ssize_t i, void *cur));

typedef Object  *(*t_it_create)(Object *self);
Object          *container_begin(Object *self);
Object          *container_end(Object *self);

#define Container_fields                \
        Object          *contained;     \
        ssize_t         contained_size;
#define Container_vtable_fields         \
        t_get_data      data;           \
        t_size          size;           \
        t_empty         empty;          \
        t_rand_insert   insert_at;      \
        t_insert        push_back;      \
        t_rand_delete   delete_at;      \
        t_delete        erase;          \
        t_access        front;          \
        t_access        back;           \
        t_rand_access   at;             \
        t_dump          dump;           \
        t_converter     convert;        \
        t_sub           sub;            \
        t_map           map;            \
        t_it_create     begin;          \
        t_it_create     end;

#define Container_definition                \
        .contained = NULL,                  \
        .contained_size = 0
#define Container_vtable_definition         \
        .data = container_data,				\
        .size = container_size,            	\
        .empty = container_empty,          	\
        .insert_at = NULL,                  \
        .push_back = container_push_back,  	\
        .delete_at = NULL,                  \
        .erase = NULL,                      \
        .front = NULL,                      \
        .back = NULL,                       \
        .at = NULL,                         \
        .dump = container_print,           	\
        .convert = container_to_type,      	\
        .sub = container_sub,              	\
        .map = container_map,              	\
        .begin = container_begin,          	\
        .end = container_end

class_declaration(Container)

t_bool  ctn_copy_ctor(Container *ctn, t_data **copy, ssize_t size);

#endif /* !CONTAINERS_H_ */
