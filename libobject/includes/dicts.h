#ifndef DICTS_H_
# define DICTS_H_

# include "arrays.h"

# define CHUNK_SIZE 50
# define LOAD_FACTOR(dict)                  ((float)dict->contained_size / (float)dict->total_size)
# define MAX_LOAD_BEFORE_REALLOC            ((float)0.70)
# define MIN_LOAD_BEFORE_REALLOC            ((float)0.20)
# define LOAD_AFTER_REALLOC                 ((float)0.40)
# define DICT_ALLOC_SIZE(s)                 (((s) / CHUNK_SIZE + 1) * CHUNK_SIZE)
# define DICT_REALLOC_SIZE(s, wanted_load)  ((s) / wanted_load)

typedef struct  s_pair
{
    t_data          data;
    unsigned char   *key;
} t_pair;

typedef t_data  *(*t_dict_get_by_key)(const Object *dict, const unsigned char *key);
t_data          *dict_get_by_key(const Object *dict, const unsigned char *key);

typedef t_bool  (*t_dict_push)(Object *self, unsigned char *key, void *data, t_type type);
t_bool          dict_push(Object *self, unsigned char *key, void *data, t_type type);

typedef t_bool  (*t_dict_remove)(Object *self, const unsigned char *key);
t_bool          dict_remove(Object *self, const unsigned char *key);

// Class and constructor declaration
#define Dict_fields                     \
        Container_fields                \
        ssize_t total_size;
#define Dict_vtable_fields              \
        Container_vtable_fields         \
        t_dict_get_by_key   get_by_key; \
        t_dict_push         push;       \
        t_dict_remove       remove;

#define Dict_definition                 \
        Container_definition,           \
        .total_size = 0
#define Dict_vtable_definition          \
        Container_vtable_definition,    \
        .push_back = NULL,              \
        .erase = dict_erase,		    \
        .get_by_key = dict_get_by_key,  \
        .push = dict_push,              \
        .remove = dict_remove

_class_declaration(Dict)
ctor_declaration(Object *, Dict, Object *class;)

t_bool  dict_erase(Object *self);
t_bool  dict_alloc(Dict *dict, ssize_t new_size);

#endif /* !DICTS_H_ */
