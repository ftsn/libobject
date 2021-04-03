#ifndef DICTS_H_
# define DICTS_H_

# include "containers.h"

# define CHUNK_SIZE 50
# define DICT_ALLOC_SIZE(s) (((s) / CHUNK_SIZE + 1) * CHUNK_SIZE)

typedef struct  s_pair
{
    char        *key;
    void        *data;
} t_pair;

typedef Object  *(*t_obj_by_key)(const Object *dict, const char *key);
Object          *_get_obj_by_key(const Object *dict, const char *key);

typedef t_bool  (*t_map_push)(Object *self, char *key, void *data, t_type type);
t_bool          _dict_push_back(Object *self, char *key, void *data, t_type type);

typedef struct
{
    Container       base;
    ssize_t         total_size;
    t_obj_by_key    get_by_key;
    t_map_push      push_back;
} Dict;

t_bool  dict_alloc(Container *dict, ssize_t new_size);
void    dict_basic_print(ssize_t i, const t_data *elem, const char *prefix);

extern Class *_dict;

#endif /* !DICTS_H_ */
