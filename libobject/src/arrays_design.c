#include <stdlib.h>
#include "arrays.h"

static t_bool	copy_ctor(Container *array, void **copy, ssize_t size)
{
  ssize_t	i;

  i = 0;
  if (size == COPY_ALL)
    {
      size = 0;
      while (copy[size])
	++size;
    }
  while (i < size)
    {
      if (array->push_back(array, copy[i]) == FALSE)
	return (FALSE);
      ++i;
    }
  return (TRUE);
}

static t_bool	_array_ctor(Object *self, va_list *args)
{
  Container	*array;
  ssize_t	nb_args;
  void		*copy;

  array = self;
  if (!(array->contained = calloc(1, sizeof(void *))))
    return (FALSE);
  if ((copy = va_arg(*args, void *)))
    if (copy_ctor(array, copy, va_arg(*args, ssize_t)) == FALSE)
      return (FALSE);
  nb_args = va_arg(*args, ssize_t);
  while (nb_args > 0)
    {
      if (array->push_back(array, va_arg(*args, void *)) == FALSE)
	return (FALSE);
      --nb_args;
    }
  return (TRUE);
}

static void	_array_dtor(Object *self, va_list *args)
{
  (void)args;
  free(((Container *)self)->contained);
  ((Container *)self)->contained = NULL;
  ((Container *)self)->contained_size = 0;
}

static Array _array_descr =
  {
    {
      {
	sizeof(Array),
	"Array",
	&_array_ctor,
	&_array_dtor
      },
      NULL,
      0,

      &_container_data,
      &_container_size,
      &_container_empty,

      &_array_insert_at,
      &_container_push_back,
      &_array_delete_at,
      &_array_erase,
      NULL,

      &_array_front,
      &_array_back,
      &_array_at,

      &_array_print,

      &_container_to_type,
      &_container_sub,
      &_container_map,

      &_container_begin,
      &_container_last
    }
  };

Class	*_array = (Class *)&_array_descr;
