#include <stdlib.h>
#include <string.h>
#include "strings.h"

static t_bool	copy_ctor(Container *string, char *copy, ssize_t size)
{
  char		*res;

  if (size == COPY_ALL)
    size = strlen(copy);
  if (!(res = calloc(size + 1, sizeof(char))))
    return (FALSE);
  memcpy(res, copy, size);
  string->contained = res;
  string->contained_size = size;
  return (TRUE);
}

static t_bool	_string_ctor(Object *self, va_list *args)
{
  Container	*string;
  ssize_t	nb_args;
  char		*copy;

  string = self;
  if ((copy = va_arg(*args, char *)))
    if (copy_ctor(string, copy, va_arg(*args, ssize_t)) == FALSE)
      return (FALSE);
  nb_args = va_arg(*args, ssize_t);
  while (nb_args > 0)
    {
      if (string->insert_at(string, va_arg(*args, void *), string->contained_size) == FALSE)
	return (FALSE);
      --nb_args;
    }
  return (TRUE);
}

static void	_string_dtor(Object *self, va_list *args)
{
  (void)args;
  free(((Container *)self)->contained);
  ((Container *)self)->contained = NULL;
}

static String _string_descr =
  {
   {
    {
     TYPE_STRING,
     sizeof(String),
     &_string_ctor,
     &_string_dtor
    },
    NULL,
    0,

    &_container_data,
    &_container_size,
    &_container_empty,

    &_string_insert_at,
    &_container_push_back,
    &_string_delete_at,
    &_string_erase,
    &_string_affect,

    &_string_front,
    &_string_back,
    &_string_at,

    &_container_print,

    &_container_to_type,
    &_container_sub,
    &_container_map,

    &_container_begin,
    &_container_last
   },
   &_string_dup,
   &_string_findstr,
   &_string_find,
   &_string_lfind,
   &_string_match,
   &_string_nmatch,
   &_string_split
  };

Class	*_string = (Class *)&_string_descr;
