#include <stdlib.h>
#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

static void	*cat_with_idx(ssize_t i, void *cur)
{
  int		size;
  char		*str;

  str = NULL;
  size = snprintf(NULL, 0, "%s %zd toto", (char *)cur, i);
  str = calloc(size + 1, sizeof(char));
  snprintf(str, size + 1, "%s %zd toto", (char *)cur, i);
  return (str);
}

int		main(int ac, char **av)
{
  (void)ac;
  (void)av;
  Container    	*array;
  Container	*mapped;
  ArrayIt	*it;

  array = new(_array, NULL, 5, "Lunkwill", "Frazou", "Roo", "Noscope", "Woklada");
  mapped = array->map(array, _array, &cat_with_idx);
  it = mapped->first(mapped);
  while (it->rvalue(it) != NULL)
    {
      printf("value: [%s]\n", it->rvalue(it));
      it->incr(it);
    }
  array->dump(array, "Array", array_basic_print, "");
  mapped->dump(mapped, "Mapped", array_basic_print, "");

  return (1);
}
