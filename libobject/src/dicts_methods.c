#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicts.h"

void	pair_basic_print(size_t i, const Object *elem, const char *prefix)
{
  printf("%s%zd)key: [%s]\tvalue: [%s]\n", prefix, i, ((t_pair *)elem)->key, ((t_pair *)elem)->data);
}

void		_dict_print(const Object *container, const char *title,
			    void (*f)(size_t i, const Object *elem, const char *prefix),
			    const char *prefix)
{
  char		**dict;
  char		*concat_prefix;
  unsigned int	i;

  dict = ((Container *)container)->contained;
  i = 0;
  if (!(concat_prefix = concat(prefix, "  ")))
    return ;
  if (title)
    printf("%s%s\n", prefix, title);
  printf("%s[\n", prefix);
  if (dict)
    while (dict[i])
      {
	f(i, dict[i], concat_prefix);
	++i;
      }
  printf("%s]\n", prefix);
  free(concat_prefix);
}

Object			*_get_obj_by_key(const Object *dict_obj, const char *key)
{
  const Container	*dict;
  t_pair		**pairs;
  size_t		i;

  dict = dict_obj;
  if (!(pairs = dict->data(dict)))
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
