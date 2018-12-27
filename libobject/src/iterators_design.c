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
  return (TRUE);
}

static void	_iterator_dtor(Object *self, va_list *args)
{
  (void)self;
  (void)args;
  free(self);
}

static Iterator _it_descr =
  {
    {
      {
	sizeof(Iterator),
	"Iterator",
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
    &_incr,
    &_decr,
    &_jump,
    &_rvalue,
    &_jmp_rvalue,
    0
  };

Class	*_it = (Class *)&_it_descr;
