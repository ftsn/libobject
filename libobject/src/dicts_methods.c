#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicts.h"
#include "lists.h"
#include "iterators.h"

static unsigned long    djb2a_hash(unsigned char *str)
{
    unsigned long       hash;
    int                 c;

    hash = 5381;
    while ((c = *str++))
        hash = hash * 33 ^ c;
    return hash;
}

void    dict_basic_print(ssize_t i, const t_data *elem, const char *prefix)
{
    (void)i;
    (void)elem;
    (void)prefix;
    //printf("%s%d)key: [%s]\tvalue: [%s]\n", prefix, (int)i, ((t_pair *)elem)->key, (char *)((t_pair *)elem)->data);
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

static void     release_contained(void **contained, ssize_t size)
{
    ssize_t     i;
    Container   *list;

    i = -1;
    while (++i < size)
    {
        list = contained[i];
        if (list)
            delete(list);
    }
    free(contained);
}

t_bool          dict_alloc(Container *dict, ssize_t new_size)
{
    ssize_t     old_ctn_size, old_total_size;
    void        **contained, **old_contained;
    Iterator    *it;
    t_data      *data;
    t_pair      *pair;

    if (!(contained = calloc(new_size + 1, sizeof(void *))))
        return (FALSE);
    old_contained = dict->contained;
    old_ctn_size = dict->contained_size;
    old_total_size = ((Dict *)dict)->total_size;
    if (!(it = dict->begin(dict)))
    {
        free(contained);
        return (FALSE);
    }
    dict->contained = contained;
    dict->contained_size = 0;
    ((Dict *)dict)->total_size = new_size;
    while (!it->reached_the_end)
    {
        data = it->dereference(it);
        pair = data->data;
        if (((Dict *)dict)->push(dict, pair->key, pair->data.data, pair->data.type) == FALSE)
        {
            release_contained(dict->contained, new_size);
            dict->contained = old_contained;
            dict->contained_size = old_ctn_size;
            ((Dict *)dict)->total_size = old_total_size;
            return (FALSE);
        }
        it->next(it);
    }
    release_contained(old_contained, old_total_size);
    ((Dict *)dict)->total_size = new_size;
    return (TRUE);
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

t_bool          _dict_push(Object *self, unsigned char *key, void *data, t_type type)
{
    Container   *self_c, *list;
    ssize_t     idx;
    t_pair      *pair;
    t_bool      existed;

    self_c = self;
    if ((float)(self_c->contained_size / ((Dict *)self)->total_size) > (float)0.05)
        {
            if (dict_alloc(self, ((Dict *)self)->total_size * 2) == FALSE)
                return (FALSE);
        }
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
    ++self_c->contained_size;
    ((void **)self_c->contained)[idx] = list;
    return (TRUE);
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
    i = -1;
    cur = list->contained;
    while (++i < list->contained_size)
    {
        if (!strcmp((char *)((t_pair *)((t_data *)cur->data)->data)->key, (char *)key))
        {
            free(((t_data *)cur->data)->data);
            list->delete_at(list, i);
            if (list->contained_size == 0)
            {
                delete(list);
                ((void **)self_c->contained)[idx] = NULL;
            }
            return (TRUE);
        }
        cur = cur->next;
        ++i;
    }
    return (TRUE);
}
