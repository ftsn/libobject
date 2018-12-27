#include <stdio.h>
#include "arrays.h"
#include "iterators.h"

int		main(void)
{
  Container    	*array_test_it;
  ArrayIt	*it;

  array_test_it = new(_array, NULL, 4, "Lunkwill", "Frazou", "Roo", "Woklada");
  it = (array_test_it)->first(array_test_it);
  while (it->rvalue(it) != NULL)
    {
      printf("value: [%s]\n", it->rvalue(it));
      it->incr(it);
    }

  return (1);
}
