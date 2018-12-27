#include <stdlib.h>
#include <string.h>
#include "iterators.h"

static t_bool	_iterator_ctor(Object *self, va_list *args)
{
  Container	*container;

  container = va_arg(*args, void *);
  memcpy((char *)self + sizeof(Class),
	 (char *)container + sizeof(Class),
	 sizeof(Container) - sizeof(Class));
  ((Iterator *)self)->cur = container->at(container, 0);
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
	&_iterator_ctor,
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
	&_iterator_ctor,
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
	"Array Iterator",
	&_iterator_ctor,
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
