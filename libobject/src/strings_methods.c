#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stringsdef.h"
#include "utils.h"

t_bool      _string_insert_at(String *self, char c, ssize_t pos)
{
    char    *res;

    if (pos < 0 || pos > self->contained_size || !(res = calloc(self->contained_size + 2, sizeof(char))))
        return (FALSE);
    if (pos > 0)
        memcpy(res, self->contained, pos);
    res[pos] = c;
    memcpy(&res[pos + 1], &((char *)self->contained)[pos], self->contained_size - pos);
    free(self->contained);
    self->contained = res;
    ++self->contained_size;
    return (TRUE);
}

t_bool  _string_push_back(String *self, char c)
{
    return (self->insert_at(self, c, self->contained_size));
}

t_bool      _string_delete_at(String *self, ssize_t pos)
{
    char    *res;

    if (pos < 0 || pos >= self->contained_size || !(res = calloc(self->contained_size, sizeof(char))))
        return (FALSE);
    memcpy(res, self->contained, pos);
    memcpy(&res[pos], &(self->contained[pos + 1]), self->contained_size - pos - 1);
    free(self->contained);
    self->contained = res;
    --self->contained_size;
    return (TRUE);
}

t_bool  _string_erase(String *self)
{
    free(self->contained);
    self->contained_size = 0;
    self->contained = NULL;
    return (TRUE);
}

t_bool      _string_affect(String *self, void *data)
{
    char    *tmp;

    tmp = self->contained;
    self->contained = str_dup(data);
    if (!self->contained && data)
    {
        self->contained = tmp;
        return (FALSE);
    }
    free(tmp);
    self->contained_size = data == NULL ? 0 : strlen(data);
    return (TRUE);
}

char    *_string_front(const String *self)
{
    return (self->contained ? &((char *)self->contained)[0] : NULL);
}

char    *_string_back(const String *self)
{
    return (self->contained ? &((char *)self->contained)[self->contained_size - 1] : NULL);
}

char    *_string_at(const String *self, ssize_t pos)
{
    return (pos < self->contained_size && self->contained ? &((char *)self->contained)[pos] : NULL);
}

void    _string_print(const String *self, const char *title)
{
    printf("%s[%s]\n", title, (char *)self->contained);
}

String  *_string_dup(const String *self)
{
    return (new(_string, self->contained, 0));
}

char    *_string_findstr(const String *self, const char *substr)
{
    return (strstr((char *)self->contained, substr));
}

char    *_string_find(const String *self, int c)
{
    return (strchr((char *)self->contained, c));
}

char    *_string_lfind(const String *self, int c)
{
    return (strrchr((char *)self->contained, c));
}

static size_t   nmatch(const char *s1, const char *s2)
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

t_bool  _string_match(const String *self, const char *compare)
{
    return (nmatch((char *)self->contained, compare) ? TRUE : FALSE);
}

size_t  _string_nmatch(const String *self, const char *compare)
{
    return (nmatch((char *)self->contained, compare));
}

Object          *_string_split(const String *self, const Class *type, const char *sep)
{
    Container   *container;
    char        *token;
    char        *strdump;

    if (!(container = new (type, NULL, 0)))
        return (NULL);
    if (!(strdump = str_dup((char *)self->contained)))
    {
        delete (container);
        return (NULL);
    }
    token = strtok(strdump, sep);
    while (token)
    {
        if (container->push_back(container, token, TYPE_CSTRING) == FALSE)
        {
            free(strdump);
            delete (container);
            return (NULL);
        }
        token = strtok(NULL, sep);
    }
    free(strdump);
    return (container);
}

char        *_string_sub(const String *self, ssize_t begin, ssize_t len)
{
    char    *s;
    ssize_t i;

    i = 0;
    if (begin < 0)
        if ((begin = self->contained_size  + begin) < 0)
            return (NULL);
    if (self->contained_size > 0 && begin >= 0 && begin >= self->contained_size)
        return (NULL);
    if (begin + len > self->contained_size)
        return (NULL);
    if (!(s = malloc(sizeof(char) * len + 1)))
        return (NULL);
    s[len] = '\0';
    while (i < len)
    {
        s[i] = self->contained[begin + i];
        ++i;
    }
    return (s);
}
