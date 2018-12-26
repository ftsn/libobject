#include <stdlib.h>
#include "arrays.h"

static void	copy_ctor(Container *array, void **copy, size_t size)
{
  size_t	i;
  size_t	j;
  void		*new_contained;

  if (size == 0) /* COPY_ALL */
    while (copy[size])
      ++size;
  size += array->contained_size;
  if (!(new_contained = calloc(size + 1, sizeof(void *))))
    return ;
  i = 0;
  while (i < array->contained_size)
    {
      ((char **)new_contained)[i] = array->at(array, i);
      ++i;
    }
  array->contained_size = size;
  j = 0;
  while (i + j < array->contained_size)
    {
      ((char **)new_contained)[i + j] = ((char **)copy)[j];
      ++j;
    }
  array->contained = new_contained;
}

static t_bool	_array_ctor(Object *self, va_list *args)
{
  Container	*array;
  size_t	nb_args;
  void		*copy;

  array = self;
  if ((copy = va_arg(*args, void *)))
    copy_ctor(array, copy, va_arg(*args, size_t));
  nb_args = va_arg(*args, size_t);
  while (nb_args > 0)
    {
      if (array->push_back(array, va_arg(*args, void *)) == FALSE)
	return (FALSE);
      --nb_args;
    }
  if (!array->contained)
    {
      if (array->push_back(array, NULL) == FALSE)
	return (FALSE);
      array->contained_size = 0;
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
      &_container_sub
    }
  };

Class	*_array = (Class *)&_array_descr;
