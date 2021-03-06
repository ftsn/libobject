#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicts.h"
#include "iterators.h"

void	dict_basic_print(ssize_t i, const t_data *elem, const char *prefix)
{
  printf("%s%d)key: [%s]\tvalue: [%s]\n", prefix, (int)i, ((t_pair *)elem)->key, (char *)((t_pair *)elem)->data);
}

Object			*_get_obj_by_key(const Object *dict_obj, const char *key)
{
  const Container	*dict;
  t_pair		**pairs;
  ssize_t		i;

  dict = dict_obj;
  if (!(pairs = dict->contained))
    return (NULL);
  i = 0;
  while (i < dict->contained_size)
    {
      if (!strcmp(pairs[i]->key, key))
	return (pairs[i]->data);
      ++i;
    }
  return (NULL);
}

t_bool		_dict_push_back(Object *self, char *key, void *data)
{
  Container	*self_c;
  t_pair	*pair;

  self_c = self;
  if (!(pair = malloc(1 * sizeof(*pair))))
    return (FALSE);
  pair->key = key;
  pair->data = data;
  return (self_c->insert_at(self_c, pair, self_c->contained_size));
}
