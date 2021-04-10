#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicts.h"
#include "lists.h"

static unsigned long    djb2a_hash(unsigned char *str)
{
    unsigned long       hash;
    int                 c;

    hash = 5381;
    while ((c = *str++))
        hash = hash * 33 ^ c;
    return hash;
}

static void     release_dict_content(Container **contained, ssize_t size)
{
    t_list_data *node;
    ssize_t     i, list_idx;

    i = 0;
    while (i < size)
    {
        list_idx = 0;
        if (contained[i])
        {
            node = contained[i]->contained;
            while (list_idx < contained[i]->contained_size)
            {
                free(((t_data *)node->data)->data);
                ((t_data *)node->data)->data = NULL;
                node = node->next;
                ++list_idx;
            }
            delete(contained[i]);
            contained[i] = NULL;
        }
        ++i;
    }
}

static void release_contained(Container **contained, ssize_t size)
{
    release_dict_content(contained, size);
    free(contained);
}

t_bool          _dict_erase(Object *self)
{
    Container   *dict;

    dict = self;
    release_dict_content(dict->contained, ((Dict *)dict)->total_size);
    return (TRUE);
}

Object              *_get_obj_by_key(const Object *dict_obj, const char *key)
{
    /*
    const Container *dict;
    t_data          **typed_pairs;
    ssize_t         i;

    dict = dict_obj;
    if (!(typed_pairs = dict->contained))
        return (NULL);
    i = 0;
    while (i < dict->contained_size)
    {
        if (!strcmp(((t_pair *)typed_pairs[i])->key, key))
            return (((t_pair *)typed_pairs[i])->data);
        ++i;
    }
    */
   (void)dict_obj;
   (void)key;
    return (NULL);
}

static t_pair   *get_pair_from_list(Container *list, unsigned char *key)
{
    ssize_t     i;
    t_list_data *cur;

    i = -1;
    cur = list->contained;
    while (++i < list->contained_size)
    {
        if (!strcmp((char *)((t_pair *)((t_data *)cur->data)->data)->key, (char *)key))
            return (((t_data *)(cur->data))->data);
        cur = cur->next;
        ++i;
    }
    return (NULL);
}

static t_bool   _dict_push_no_resizing(Object *self, unsigned char *key, void *data, t_type type)
{
    Container   *self_c, *list;
    ssize_t     idx;
    t_pair      *pair;
    t_bool      existed;

    self_c = self;
    idx = djb2a_hash(key) % ((Dict *)self)->total_size;
    if ((list = ((void **)self_c->contained)[idx]) == NULL)
    {
        if (!(list = new(_dbl_list, NULL, 0)))
            return (FALSE);
    }
    pair = get_pair_from_list(list, key);
    existed = pair ? TRUE : FALSE;
    if (existed == FALSE)
    {
        if ((pair = malloc(sizeof(t_pair))) == NULL)
        {
            delete(list);
            return (FALSE);
        }
    }
    pair->key = key;
    pair->data.type = type;
    pair->data.data = data;
    if (existed == FALSE)
    {
        if (list->push_back(list, pair, TYPE_PAIR) == FALSE)
        {
            free(pair);
            delete(list);
            return (FALSE);
        }
    }
    if (existed == FALSE)
        ++self_c->contained_size;
    ((void **)self_c->contained)[idx] = list;
    return (TRUE);
}

static t_bool   calculate_new_idx_and_assign(Container *dict, Container **old_contained, ssize_t old_total_size)
{
    ssize_t     i, list_idx;
    Container   *list;
    t_list_data *node;
    t_pair      *pair;

    i = -1;
    while (++i < old_total_size)
    {
        list = old_contained[i];
        list_idx = 0;
        if (list)
        {
            node = list->contained;
            while (list_idx < list->contained_size)
            {
                pair = ((t_data *)node->data)->data;
                if (_dict_push_no_resizing(dict, pair->key, pair->data.data, pair->data.type) == FALSE)
                    return (FALSE);
                node = node->next;
                ++list_idx;
            }
        }
    }
    return (TRUE);
}

t_bool          dict_alloc(Container *dict, ssize_t new_size)
{
    ssize_t     old_ctn_size, old_total_size;
    Container   **contained, **old_contained;

    if (!(contained = calloc(new_size + 1, sizeof(void *))))
        return (FALSE);
    old_contained = dict->contained;
    old_ctn_size = dict->contained_size;
    old_total_size = ((Dict *)dict)->total_size;
    dict->contained = contained;
    dict->contained_size = 0;
    ((Dict *)dict)->total_size = new_size;
    if (calculate_new_idx_and_assign(dict, old_contained, old_total_size) == FALSE)
    {
        release_contained(dict->contained, new_size);
        dict->contained = old_contained;
        dict->contained_size = old_ctn_size;
        ((Dict *)dict)->total_size = old_total_size;
        return (FALSE);
    }
    release_contained(old_contained, old_total_size);
    ((Dict *)dict)->total_size = new_size;
    return (TRUE);
}

t_bool          _dict_push(Object *self, unsigned char *key, void *data, t_type type)
{
    Container   *self_c;

    self_c = self;
    if ((float)self_c->contained_size / (float)((Dict *)self)->total_size > MAX_LOAD_BEFORE_REALLOC)
    {
        if (dict_alloc(self, DICT_REALLOC_SIZE(((Dict *)self)->total_size)) == FALSE)
            return (FALSE);
    }
    return (_dict_push_no_resizing(self, key, data, type));
}

t_bool          _dict_remove(Object *self, unsigned char *key)
{
    Container   *self_c, *list;
    t_list_data *cur;
    ssize_t     i, idx;

    self_c = self;
    idx = djb2a_hash(key) % ((Dict *)self)->total_size;
    if ((list = ((void **)self_c->contained)[idx]) == NULL)
        return (FALSE);
    i = 0;
    cur = list->contained;
    while (i < list->contained_size)
    {
        if (!strcmp((char *)((t_pair *)((t_data *)cur->data)->data)->key, (char *)key))
        {
            free(((t_data *)cur->data)->data);
            list->delete_at(list, i);
            if (list->contained_size == 0)
            {
                delete(list);
                ((void **)self_c->contained)[idx] = NULL;
                --self_c->contained_size;
            }
            break ;
        }
        cur = cur->next;
        ++i;
    }
    if (((Dict *)self)->total_size > CHUNK_SIZE && (float)self_c->contained_size / (float)((Dict *)self)->total_size < MIN_LOAD_BEFORE_REALLOC)
    {
        if (dict_alloc(self, DICT_ALLOC_SIZE(self_c->contained_size)) == FALSE)
            return (FALSE);
    }
    return (TRUE);
}
