#include <stdio.h>
#include "lists.h"
#include "iterators.h"

int		main(int ac, char **av)
{
  Container    	*array_test_it;
  ArrayIt	*it;

  (void)ac;
  (void)av;
  array_test_it = new(_spl_list, NULL, 5, "Lunkwill", "Frazou", "Roo", "Noscope", "Woklada");


  array_test_it->deleteAt(array_test_it, 4000);
  it = (array_test_it)->first(array_test_it);
  while (it->rvalue(it) != NULL)
    {
      printf("value: [%s]\n", it->rvalue(it));
      it->incr(it);
    }
  array_test_it->deleteAt(array_test_it, 4);

  array_test_it->dump(array_test_it, "Without woklada", _list_basic_print, "");
  array_test_it->deleteAt(array_test_it, 0);
  array_test_it->dump(array_test_it, "Without Lunkwill", _list_basic_print, "");
  array_test_it->deleteAt(array_test_it, 1);
  array_test_it->dump(array_test_it, "Without Roo", _list_basic_print, "");

  return (1);
}
