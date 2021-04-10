#ifndef DICTS_H_
# define DICTS_H_

# include "containers.h"

# define CHUNK_SIZE 50
# define DICT_ALLOC_SIZE(s) (((s) / CHUNK_SIZE + 1) * CHUNK_SIZE)
# define DICT_REALLOC_SIZE(s) ((s) * 2)
# define MAX_LOAD_BEFORE_REALLOC ((float)0.05)
# define MIN_LOAD_BEFORE_REALLOC ((float)0.40)

typedef struct  s_pair
{
    t_data          data;
    unsigned char   *key;
} t_pair;

typedef Object  *(*t_obj_by_key)(const Object *dict, const char *key);
Object          *_get_obj_by_key(const Object *dict, const char *key);

typedef t_bool  (*t_dict_push)(Object *self, unsigned char *key, void *data, t_type type);
t_bool          _dict_push(Object *self, unsigned char *key, void *data, t_type type);

typedef t_bool  (*t_dict_remove)(Object *self, unsigned char *key);
t_bool          _dict_remove(Object *self, unsigned char *key);

typedef struct
{
    Container       base;
    ssize_t         total_size;
    t_obj_by_key    get_by_key;
    t_dict_push     push;
    t_dict_remove   remove;
} Dict;

t_bool  _dict_erase(Object *self);
t_bool  dict_alloc(Container *dict, ssize_t new_size);

extern Class *_dict;

#endif /* !DICTS_H_ */
