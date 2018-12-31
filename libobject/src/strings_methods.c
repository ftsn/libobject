#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

t_bool		_string_insert_at(Object *string, void *data, ssize_t pos)
{
  Container	*self;
  char		*res;

  self = string;
  if (!(res = calloc(self->contained_size + 2, sizeof(char))))
    return (FALSE);
  if (pos > 0)
    memcpy(res, self->contained, pos);
  res[pos] = *(char *)data;
  memcpy(&res[pos + 1], &((char *)self->contained)[pos], self->contained_size - pos);
  free(self->contained);
  self->contained = res;
  ++self->contained_size;
  return (TRUE);
}

t_bool		_string_delete_at(Object *string, ssize_t pos)
{
  Container	*self;
  char		*res;

  self = string;
  if (!(res = calloc(self->contained_size, sizeof(char))))
    return (FALSE);
  memcpy(res, self->contained, pos);
  memcpy(&res[pos], &((char *)self->contained)[pos + 1], self->contained_size - pos - 1);
  free(self->contained);
  self->contained = res;
  --((Container *)string)->contained_size;
  return (TRUE);
}

t_bool		_string_erase(Object *string)
{
  Container	*self;

  self = string;
  if (self->empty(self) == TRUE || self->delete_at(self, 0) == FALSE)
    return (FALSE);
  self->erase(self);
  return (TRUE);
}

void		_string_affect(Object *string, void *data)
{
  Container	*self;

  self = string;
  free(self->contained);
  self->contained = data;
  self->contained_size = strlen(data);
}

Object			*_string_front(const Object *string)
{
  const Container	*container;

  container = string;
  return (container->contained ? &((char *)container->contained)[0] : NULL);
}

Object			*_string_back(const Object *string)
{
  const Container	*container;

  container = string;
  return (container->contained ?
	  &((char *)container->contained)[container->contained_size - 1] :
	  NULL);
}

Object			*_string_at(const Object *string, ssize_t pos)
{
  const Container	*container;

  container = string;
  return (pos < container->contained_size && container->contained ?
	  &((char *)container->contained)[pos] :
	  NULL);
}

void	_string_basic_print(ssize_t i, const Object *elem, const char *prefix)
{
  printf("%s[%s]\n", prefix, (char *)elem);
  (void)i;
}

void	_string_print(const Object *self, const char *title,
		      void (*f)(ssize_t i, const Object *elem, const char *prefix),
		      const char *prefix)
{
  char	*concat_prefix;

  if (!(concat_prefix = concat(prefix, "  ")))
    return ;
  if (title)
    printf("%s%s\n", prefix, title);
  f(0, ((Container *)self)->contained, concat_prefix);
  free(concat_prefix);
}

Object		*_string_dup(const Object *self)
{
  return (new(_string, ((Container *)self)->contained, 0));
}

ssize_t	_string_findstr(const Object *self, const char *substr)
{
  char	*res;

  res = strstr((char *)((Container *)self)->contained, substr);
  return (res ? ((Container *)self)->contained_size - (strlen(res) - 1) : 0);
}

ssize_t	_string_find(const Object *self, int c)
{
  char	*res;

  res = strchr((char *)((Container *)self)->contained, c);
  return (res ? ((Container *)self)->contained_size - (strlen(res) - 1) : 0);
}

ssize_t	_string_lfind(const Object *self, int c)
{
  char	*res;

  res = strrchr((char *)((Container *)self)->contained, c);
  return (res ? ((Container *)self)->contained_size - (strlen(res) - 1) : 0);
}

static size_t	nmatch(const char *s1, const char *s2)
{
  if (*s1 != '\0' && *s2 == '*')
    return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
  if (*s1 == '\0' && *s2 == '*')
    return (nmatch(s1, s2 + 1));
  if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
    return (nmatch(s1 + 1, s2 + 1));
  if (*s1 == *s2 && *s1 == '\0' && *s2 == '\0')
    return (1);
  return (0);
}

t_bool	_string_match(const Object *self, const char *compare)
{
  return (nmatch(((Container *)self)->contained, compare) ? TRUE : FALSE);
}

size_t	_string_nmatch(const Object *self, const char *compare)
{
  return (nmatch(((Container *)self)->contained, compare));
}

Object		*_string_split(const Object *self, const Class *type, const char *sep)
{
  Container	*container;
  char		*token;
  char		*strdump;

  if (!(container = new(type, NULL, 0)))
    return (NULL);
  if (!(strdump = str_dup(((Container *)self)->contained)))
    {
      delete(container);
      return (NULL);
    }
  token = strtok(strdump, sep);
  while (token)
    {
      if (container->push_back(container, token) == FALSE)
	{
	  free(strdump);
	  delete(container);
	  return (NULL);
	}
      token = strtok(NULL, sep);
    }
  free(strdump);
  return (container);
}
