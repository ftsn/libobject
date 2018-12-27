#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"

static void	copy_array(void **dest, void **src, void *data, int pos)
{
  int		i;
  int		j;

  i = 0;
  if (!dest || !src)
    return ;
  while (src[i] && i < pos)
    {
      dest[i] = src[i];
      ++i;
    }
  j = i;
  dest[i++] = data;
  while (src[j])
    dest[i++] = src[j++];
}

t_bool		_array_insert_at(Object *container, void *data, int pos)
{
  void		**res;
  Container	*self;

  self = container;
  if (self->empty(self) == TRUE)
    {
      if (!(self->contained = malloc(2 * sizeof(void *))))
	return (FALSE);
      ((void **)self->contained)[0] = data;
      ((void **)self->contained)[1] = NULL;
    }
  else
    {
      if (!(res = malloc(sizeof(void *) * (self->contained_size + 2))))
	return (FALSE);
      copy_array(res, self->contained, data, pos);
      res[self->contained_size + 1] = NULL;
      free(self->contained);
      self->contained = res;
    }
  ++self->contained_size;
  return (TRUE);
}

t_bool		_array_delete_at(Object *container, int pos)
{
  Container	*self;
  void		**res;
  int		i;
  int		j;

  self = container;
  if (self->empty(self) == TRUE)
    return (TRUE);
  if (pos >= (int)self->contained_size)
    pos = self->contained_size - 1;
  if (!(res = malloc(self->contained_size * sizeof(void *))))
    return (FALSE);
  i = -1;
  while (((void **)self->contained)[++i] && i < pos)
    res[i] = ((void **)self->contained)[i];
  j = i;
  ++i;
  while (((void **)self->contained)[i])
    res[j++] = ((void **)self->contained)[i++];
  res[j] = NULL;
  free(self->contained);
  self->contained = res;
  --self->contained_size;
  return (TRUE);
}

t_bool		_array_erase(Object *container)
{
  Container	*self;

  self = container;
  if (self->empty(self) == TRUE)
    return (TRUE);
  self->deleteAt(self, 0);
  self->erase(self);
  return (TRUE);
}

Object			*_array_front(const Object *array)
{
  const Container	*container;

  container = array;
  return (container->contained ? ((void **)container->contained)[0] : NULL);
}

Object			*_array_back(const Object *array)
{
  const Container	*container;

  container = array;
  return (container->contained ?
	  ((void **)container->contained)[container->contained_size - 1] :
	  NULL);
}

Object			*_array_at(const Object *self, size_t pos)
{
  const Container	*container;

  container = self;
  return (pos < container->contained_size && container->contained ?
	  ((void **)container->contained)[pos] :
	  NULL);
}

void	array_basic_print(size_t i, const Object *elem, const char *prefix)
{
  printf("%s%u: [%s]\n", prefix, (unsigned int)i, (char *)elem);
}

void		_array_print(const Object *container, const char *title,
			     void (*f)(size_t i, const Object *elem, const char *prefix),
			     const char *prefix)
{
  char		**array;
  char		*concat_prefix;
  unsigned int	i;

  array = ((Container *)container)->contained;
  i = 0;
  if (!(concat_prefix = concat(prefix, "  ")))
    return ;
  if (title)
    printf("%s%s\n", prefix, title);
  printf("%s[\n", prefix);
  if (array)
    while (array[i])
      {
	f(i, array[i], concat_prefix);
	++i;
      }
  printf("%s]\n", prefix);
  free(concat_prefix);
}
