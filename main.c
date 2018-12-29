#include <stdio.h>
#include "lists.h"
#include "iterators.h"

int		main(int ac, char **av)
{
  Container    	*array_test_it;
  ArrayIt	*it;

  (void)ac;
  (void)av;
  array_test_it = new(_spl_list, NULL, 4, "Lunkwill", "Frazou", "Roo", "Woklada");
  it = (array_test_it)->first(array_test_it);
  while (it->rvalue(it) != NULL)
    {
      printf("value: [%s]\n", it->rvalue(it));
      it->incr(it);
    }

  return (1);
}
