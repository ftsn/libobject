#include <stdlib.h>
#include "types.h"

t_data		*raw_data_to_typed(void *raw, t_type type)
{
  t_data	*typed;

  if (!(typed = malloc(sizeof(t_data) * 1)))
    return (NULL);
  typed->type = type;
  typed->data = raw;
  return (typed);
}

void		free_typed_array(t_data **array)
{
  size_t	i;

  i = 0;
  while (array[i])
    free(array[i++]);
  free(array);
}

t_data		**init_typed_array(size_t nmemb)
{
  return (calloc(nmemb + 1, sizeof(t_data *)));
}

t_data		**cstrings_to_data_array(char **array)
{
  size_t	i;
  t_data	**data_array;

  i = 0;
  while (array[i] != NULL)
    ++i;
  if (!((data_array = malloc(sizeof(t_data *) * (i + 1)))))
    return (NULL);
  i = 0;
  while (array[i] != NULL)
    {
      if (!(data_array[i] = raw_data_to_typed(array[i], TYPE_CSTRING)))
	return (NULL);
      ++i;
    }
  data_array[i] = NULL;
  return (data_array);
}
