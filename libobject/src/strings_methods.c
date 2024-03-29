#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stringsdef.h"
#include "utils.h"
#include "iterators.h"

Object  *string_data(const String *self)
{
    return (self->contained);
}

ssize_t string_size(const String *self)
{
    return (self->contained_size);
}

t_bool  string_empty(const String *self)
{
    return (self->contained_size == 0 ? TRUE : FALSE);
}

t_bool      string_insert_at(String *self, char c, ssize_t pos)
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

t_bool  string_push_back(String *self, char c)
{
    return (self->vtable->insert_at(self, c, self->contained_size));
}

t_bool      string_delete_at(String *self, ssize_t pos)
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

t_bool  string_erase(String *self)
{
    free(self->contained);
    self->contained_size = 0;
    self->contained = NULL;
    return (TRUE);
}

t_bool      string_affect(String *self, void *data)
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

char    *string_front(const String *self)
{
    return (self->contained ? self->contained : NULL);
}

char    *string_back(const String *self)
{
    if (!self->contained)
        return (NULL);
    return (self->contained_size > 0 ? &((char *)self->contained)[self->contained_size - 1] : self->contained);
}

char    *string_at(const String *self, ssize_t pos)
{
    if (!self->contained || pos < 0 || (pos >= self->contained_size && self->contained_size > 0))
        return (NULL);
    return (pos < self->contained_size ? &((char *)self->contained)[pos] : self->contained);
}

void    string_print(const String *self, const char *title)
{
    printf("%s[%s]\n", title, (char *)self->contained);
}

Object  *string_begin(Object *self)
{
    return (generate_it(self, BEGIN));
}

Object  *string_end(Object *self)
{
    return (generate_it(self, END));
}

char    *string_findstr(const String *self, const char *substr)
{

    return (self->contained && substr ? strstr((char *)self->contained, substr) : NULL);
}

char    *string_find(const String *self, int c)
{
    return (self->contained ? strchr((char *)self->contained, c) : NULL);
}

char    *string_lfind(const String *self, int c)
{
    return (self->contained ? strrchr((char *)self->contained, c) : NULL);
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

t_bool  string_match(const String *self, const char *compare)
{
    return (self->contained && compare && nmatch((char *)self->contained, compare) ? TRUE : FALSE);
}

ssize_t  string_nmatch(const String *self, const char *compare)
{
    return (self->contained && compare ? (ssize_t)nmatch((char *)self->contained, compare) : -1);
}

Object          *string_split(const String *self, const Class *type, const char *sep)
{
    Container   *container;
    char        *token;
    char        *strdump;

    if (!self->contained || !sep || !(container = shallow_new_obj(type)))
        return (NULL);
    strdump = self->contained;
    token = strtok(strdump, sep);
    while (token)
    {
        if (container->vtable->push_back(container, token, TYPE_CSTRING) == FALSE)
        {
            free(strdump);
            delete(container);
            return (NULL);
        }
        token = strtok(NULL, sep);
    }
    return (container);
}

char        *string_sub(const String *self, ssize_t begin, ssize_t len)
{
    char    *s;
    ssize_t i;

    i = 0;
    if (!self->contained)
        return (NULL);
    if (begin < 0)
        if ((begin = self->contained_size + begin) < 0)
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
