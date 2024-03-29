#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicts.h"
#include "lists.h"

static unsigned long    djb2a_hash(const unsigned char *str)
{
    unsigned long       hash;
    int                 c;

    hash = 5381;
    while ((c = *str++))
        hash = hash * 33 ^ c;
    return hash;
}

static void     release_dict_content(DblList **contained, ssize_t size)
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

static void release_contained(DblList **contained, ssize_t size)
{
    release_dict_content(contained, size);
    free(contained);
}

t_bool      dict_erase(Object *self)
{
    Dict    *dict;

    dict = self;
    release_dict_content(dict->contained, dict->total_size);
    return (TRUE);
}

t_data          *dict_get_by_key(const Object *dict_obj, const unsigned char *key)
{
    const Dict  *dict = dict_obj;
    DblList     *list;
    t_list_data *cur;
    ssize_t     i, idx;
    t_pair      *pair;

    idx = djb2a_hash(key) % dict->total_size;
    if ((list = ((void **)dict->contained)[idx]) == NULL)
        return (NULL);
    i = 0;
    cur = list->contained;
    while (i < list->contained_size)
    {
        pair = ((t_data *)cur->data)->data;
        if (!strcmp((char *)pair->key, (char *)key))
            return (&pair->data);
        cur = cur->next;
        ++i;
    }
    return (NULL);
}

static t_pair   *get_pair_from_list(DblList *list, unsigned char *key)
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
    Dict        *dict;
    DblList     *list;
    ssize_t     idx;
    t_pair      *pair;
    t_bool      existed;

    dict = self;
    idx = djb2a_hash(key) % dict->total_size;
    if ((list = ((void **)dict->contained)[idx]) == NULL)
    {
        if (!(list = new_obj(DblList)))
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
        if (list->vtable->push_back(list, pair, TYPE_PAIR) == FALSE)
        {
            free(pair);
            delete(list);
            return (FALSE);
        }
    }
    if (existed == FALSE)
        ++dict->contained_size;
    ((void **)dict->contained)[idx] = list;
    return (TRUE);
}

static t_bool   calculate_new_idx_and_assign(Dict *dict, DblList **old_contained, ssize_t old_total_size)
{
    ssize_t     i, list_idx;
    DblList     *list;
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

t_bool      dict_alloc(Dict *dict, ssize_t new_size)
{
    ssize_t old_ctn_size, old_total_size;
    DblList **contained, **old_contained;

    if (!(contained = calloc(new_size + 1, sizeof(void *))))
        return (FALSE);
    old_contained = dict->contained;
    old_ctn_size = dict->contained_size;
    old_total_size = dict->total_size;
    dict->contained = contained;
    dict->contained_size = 0;
    dict->total_size = new_size;
    if (calculate_new_idx_and_assign(dict, old_contained, old_total_size) == FALSE)
    {
        release_contained(dict->contained, new_size);
        dict->contained = old_contained;
        dict->contained_size = old_ctn_size;
        dict->total_size = old_total_size;
        return (FALSE);
    }
    release_contained(old_contained, old_total_size);
    dict->total_size = new_size;
    return (TRUE);
}

t_bool      dict_push(Object *self, unsigned char *key, void *data, t_type type)
{
    Dict    *dict;

    dict = self;
    if (LOAD_FACTOR(dict) > MAX_LOAD_BEFORE_REALLOC)
    {
        if (dict_alloc(self, DICT_REALLOC_SIZE(dict->contained_size, LOAD_AFTER_REALLOC)) == FALSE)
            return (FALSE);
    }
    return (_dict_push_no_resizing(self, key, data, type));
}

t_bool          dict_remove(Object *self, const unsigned char *key)
{
    Dict        *dict;
    DblList     *list;
    t_list_data *cur;
    ssize_t     i, idx;

    dict = self;
    idx = djb2a_hash(key) % dict->total_size;
    if ((list = ((void **)dict->contained)[idx]) == NULL)
        return (FALSE);
    i = 0;
    cur = list->contained;
    while (i < list->contained_size)
    {
        if (!strcmp((char *)((t_pair *)((t_data *)cur->data)->data)->key, (char *)key))
        {
            free(((t_data *)cur->data)->data);
            list->vtable->delete_at(list, i);
            if (list->contained_size == 0)
            {
                delete(list);
                ((void **)dict->contained)[idx] = NULL;
                --dict->contained_size;
            }
            break ;
        }
        cur = cur->next;
        ++i;
    }
    if (dict->total_size > CHUNK_SIZE && LOAD_FACTOR(dict) < MIN_LOAD_BEFORE_REALLOC)
    {
        if (dict_alloc(self, DICT_REALLOC_SIZE(dict->contained_size, LOAD_AFTER_REALLOC)) == FALSE)
            return (FALSE);
    }
    return (TRUE);
}
