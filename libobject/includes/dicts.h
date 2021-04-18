#ifndef DICTS_H_
# define DICTS_H_

# include "containers.h"

# define CHUNK_SIZE 50
# define LOAD_FACTOR(dict_obj)              ((float)((Container *)dict_obj)->contained_size / (float)((Dict *)dict_obj)->total_size)
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

typedef t_data  *(*t_obj_by_key)(const Object *dict, const unsigned char *key);
t_data          *_get_obj_by_key(const Object *dict, const unsigned char *key);

typedef t_bool  (*t_dict_push)(Object *self, unsigned char *key, void *data, t_type type);
t_bool          _dict_push(Object *self, unsigned char *key, void *data, t_type type);

typedef t_bool  (*t_dict_remove)(Object *self, const unsigned char *key);
t_bool          _dict_remove(Object *self, const unsigned char *key);

// Class and constructor declaration
class_declaration(Dict,
    Container       base;
    ssize_t         total_size;
    t_obj_by_key    get_by_key;
    t_dict_push     push;
    t_dict_remove   remove;
)
ctor_declaration(Object *, Dict, Object *class;)

t_bool  _dict_erase(Object *self);
t_bool  dict_alloc(Container *dict, ssize_t new_size);

#endif /* !DICTS_H_ */
