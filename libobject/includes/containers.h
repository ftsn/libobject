#ifndef CONTAINERS_H_
# define CONTAINERS_H_

# include "object.h"

# define COPY_ALL   ((ssize_t)-1)

typedef Object  *(*t_get_data)(const Object *self);
Object          *_container_data(const Object *self);

typedef ssize_t (*t_size)(const Object *self);
ssize_t		    _container_size(const Object *self);

typedef t_bool  (*t_empty)(const Object *self);
t_bool          _container_empty(const Object *self);

typedef t_bool  (*t_rand_insert)(Object *self, void *data, t_type type, ssize_t pos);
t_bool          _container_insert_at(Object *self, void *data, t_type type, ssize_t pos);

typedef t_bool  (*t_insert)(Object *self, void *data, t_type type);
t_bool          _container_push_back(Object *self, void *data, t_type type);

typedef t_bool  (*t_rand_delete)(Object *self, ssize_t pos);

typedef t_bool  (*t_delete)(Object *self);

typedef void    (*t_affect)(Object *self, void *data);

typedef Object  *(*t_access)(const Object *container);

typedef Object  *(*t_rand_access)(const Object *contained, ssize_t pos);

typedef void    (*t_dump)(Object *self, const char *title, void (*f)(const t_data *elem, const char *prefix), const char *prefix);
void            _container_print(Object *container, const char *title, void (*f)(const t_data *elem, const char *prefix), const char *prefix);
void            typed_basic_print(const t_data *elem, const char *prefix);

typedef Object  *(*t_converter)(Object *self, Class *type);
Object          *_container_to_type(Object *self, Class *type);

typedef Object  *(*t_sub)(Object *self, Class *type, ssize_t begin, ssize_t len);
Object          *_container_sub(Object *self, Class *type, ssize_t begin, ssize_t len);

typedef Object  *(*t_map)(Object *self, Class *type, t_data (*fptr)(ssize_t i, void *cur));
Object          *_container_map(Object *self, Class *type, t_data (*fptr)(ssize_t i, void *cur));

typedef Object  *(*t_it_create)(Object *self);
Object          *_container_begin(Object *self);
Object          *_container_end(Object *self);

#define Container_fields                \
        Object          *contained;     \
                                        \
        ssize_t         contained_size; \
        t_get_data      data;           \
                                        \
        t_size          size;           \
        t_empty         empty;          \
                                        \
        t_rand_insert   insert_at;      \
        t_insert        push_back;      \
        t_rand_delete   delete_at;      \
        t_delete        erase;          \
        t_affect        affect;         \
                                        \
        t_access        front;          \
        t_access        back;           \
        t_rand_access   at;             \
                                        \
        t_dump          dump;           \
                                        \
        t_converter     convert;        \
        t_sub           sub;            \
        t_map           map;            \
                                        \
        t_it_create     begin;          \
        t_it_create     end;

class_declaration(Container)

t_bool  ctn_copy_ctor(Container *ctn, t_data **copy, ssize_t size);

#endif /* !CONTAINERS_H_ */
