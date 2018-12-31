#include "arrays.h"

int		main(int ac, char **av)
{
  Container	*a, *b;

  a = new(_array, av, COPY_ALL, 0);
  b = a->sub(a, _array, 0, 2);
  b->dump(b, "Array sub", array_basic_print, "");
  delete(a);
  delete(b);

  (void)ac;

  return (1);
}
