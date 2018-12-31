#include <stdlib.h>
#include <stdio.h>
#include "lists.h"
#include "arrays.h"
#include "iterators.h"

/*static void	*cat_with_idx(ssize_t i, void *cur)
{
  (void)i;
  (void)cur;
  return (cur);
}*/

int		main(int ac, char **av)
{
  (void)ac;
  (void)av;
  Container    	*list;
  Container	*mapped;
  ListIt	*it;

  list = new(_dbl_clist, NULL, 5, "Lunkwill", "Frazou", "Roo", "Noscope", "Woklada");
  mapped = list->convert(list, _array);
  it = mapped->first(mapped);
  while (it->rvalue(it) != NULL)
    {
      printf("value: [%s]\n", (char *)it->rvalue(it));
      it->incr(it);
    }
  list->dump(list, "Array", _list_basic_print, "");
  mapped->dump(mapped, "Mapped", array_basic_print, "");
  delete((void **)&it);
  delete((void **)&list);
  delete((void **)&mapped);
  

  return (1);
}
