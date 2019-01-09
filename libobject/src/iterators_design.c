#include <stdlib.h>
#include <string.h>
#include "iterators.h"
#include "lists.h"

static t_bool	_ra_iterator_ctor(Object *self, va_list *args)
{
  Container	*container;
  ssize_t      	idx_start;

  container = va_arg(*args, void *);
  memcpy((char *)self + sizeof(Class),
	 (char *)container + sizeof(Class),
	 sizeof(Container) - sizeof(Class));
  idx_start = va_arg(*args, ssize_t);
  if (idx_start == REVERSE)
    idx_start = container->contained_size - 1;
  ((Iterator *)self)->pos = idx_start;
  ((Iterator *)self)->cur = container->at(container, idx_start);
  return (TRUE);
}

static void	_iterator_dtor(Object *self, va_list *args)
{
  (void)self;
  (void)args;
}

static ArrayIt _array_it_descr =
  {
   {
    {
     TYPE_ARRAY_ITERATOR,
     sizeof(Iterator),
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
     TYPE_STRING_ITERATOR,
     sizeof(Iterator),
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
     TYPE_DICT_ITERATOR,
     sizeof(Iterator),
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
     TYPE_LIST_ITERATOR,
     sizeof(Iterator),
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
