#include <stdlib.h>
#include <string.h>
#include "iterators.h"
#include "lists.h"

static t_bool	_ra_iterator_ctor(Object *self, va_list *args)
{
  Container	*container;
  int		idx_start;

  container = va_arg(*args, void *);
  memcpy((char *)self + sizeof(Class),
	 (char *)container + sizeof(Class),
	 sizeof(Container) - sizeof(Class));
  idx_start = va_arg(*args, int);
  if (idx_start == REVERSE)
    idx_start = (int)container->size(container) - 1;
  ((Iterator *)self)->pos = idx_start;
  ((Iterator *)self)->cur = container->at(container, idx_start);
  return (TRUE);
}

static t_bool	_list_iterator_ctor(Object *self, va_list *args)
{
  List	        *container;
  int		idx_start;

  container = va_arg(*args, void *);
  memcpy((char *)self + sizeof(Class),
	 (char *)container + sizeof(Class),
	 sizeof(Container) - sizeof(Class));
  idx_start = va_arg(*args, int);
  if (idx_start == REVERSE)
    idx_start = (int)((Container *)container)->size(container) - 1;
  ((Iterator *)self)->pos = idx_start;
  ((Iterator *)self)->cur = container->nth_node(container, idx_start);
  return (TRUE);
}

static void	_iterator_dtor(Object *self, va_list *args)
{
  (void)self;
  (void)args;
  free(self);
}

static ArrayIt _array_it_descr =
  {
    {
      {
	sizeof(Iterator),
	"Array Iterator",
	&_ra_iterator_ctor,
	&_iterator_dtor
      },
      NULL,
      0,

      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,

      NULL,

      NULL,
      NULL
    },
    NULL,
    &_ra_incr,
    &_ra_decr,
    &_ra_jump,
    &_ra_rvalue,
    &_ra_jmp_rvalue,
    0
  };

Class	*_array_it = (Class *)&_array_it_descr;

static StringIt _string_it_descr =
  {
    {
      {
	sizeof(Iterator),
	"String Iterator",
	&_ra_iterator_ctor,
	&_iterator_dtor
      },
      NULL,
      0,

      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,

      NULL,

      NULL,
      NULL
    },
    NULL,
    &_ra_incr,
    &_ra_decr,
    &_ra_jump,
    &_ra_rvalue,
    &_ra_jmp_rvalue,
    0
  };

Class	*_string_it = (Class *)&_string_it_descr;

static DictIt _dict_it_descr =
  {
    {
      {
	sizeof(Iterator),
	"Dict Iterator",
	&_ra_iterator_ctor,
	&_iterator_dtor
      },
      NULL,
      0,

      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,

      NULL,

      NULL,
      NULL
    },
    NULL,
    &_ra_incr,
    &_ra_decr,
    &_ra_jump,
    &_ra_rvalue,
    &_ra_jmp_rvalue,
    0
  };

Class	*_dict_it = (Class *)&_dict_it_descr;

static ListIt _list_it_descr =
  {
    {
      {
	sizeof(Iterator),
	"List Iterator",
	&_list_iterator_ctor,
	&_iterator_dtor
      },
      NULL,
      0,

      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,
      NULL,
      NULL,

      NULL,
      NULL,
      NULL,

      NULL,

      NULL,
      NULL
    },
    NULL,
    &_list_incr,
    &_list_decr,
    &_list_jump,
    &_list_rvalue,
    &_list_jmp_rvalue,
    0
  };

Class	*_list_it = (Class *)&_list_it_descr;
