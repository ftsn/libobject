#include <stdlib.h>
#include <string.h>
#include "utils.h"

char	*concat(const char *dest, const char *src)
{
  char		*res;
  ssize_t	i;
  ssize_t	j;

  i = j = 0;
  if ((res = calloc(strlen(dest) + strlen(src) + 1, sizeof(char))) == NULL)
    return (NULL);
  while (dest[i] != '\0')
    {
      res[i] = dest[i];
      ++i;
    }
  while (src[j] != '\0')
    res[i++] = src[j++];
  return (res);
}

/*
** -ansi removes the system strdup
*/
char	*str_dup(const char *str)
{
  char	*res;

  if (!str)
    return (NULL);
  if (!(res = calloc(strlen(str) + 1, sizeof(char))) || !str)
    return (NULL);
  memcpy(res, str, strlen(str));
  return (res);
}
