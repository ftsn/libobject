#include <stdlib.h>
#include "arrays.h"

int		main(int ac, char **av)
{
  Container	*a, *b;
  t_data	**data_array1, **data_array2;
  int		nb;

  data_array2 = init_typed_array(2);
  data_array2[0] = raw_data_to_typed("Giga recursed test", TYPE_CSTRING);
  data_array2[1] = raw_data_to_typed("?", TYPE_CHAR);
  b = new(_array, data_array2, COPY_ALL, 0);
  b->dump(b, "Array b", array_basic_print, "");

  nb = 666;
  data_array1 = init_typed_array(4);
  data_array1[0] = raw_data_to_typed("Giga test", TYPE_CSTRING);
  data_array1[1] = raw_data_to_typed(b, TYPE_ARRAY);
  data_array1[2] = raw_data_to_typed(&nb, TYPE_INT);
  data_array1[3] = raw_data_to_typed("!", TYPE_CHAR);
  a = new(_array, data_array1, COPY_ALL, 0);
  a->dump(a, "Array a", array_basic_print, "");

  free_typed_array(data_array1);
  free_typed_array(data_array2);

  delete(a);
  delete(b);

  (void)ac;
  (void)av;

  return (1);
}
