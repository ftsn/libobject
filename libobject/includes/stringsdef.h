#ifndef STRINGSDEF_H_
# define STRINGSDEF_H_

# include "containers.h"

typedef Object  *(*t_dup)(const Object *self);
Object          *_string_dup(const Object *self);

typedef ssize_t (*t_str_findstr)(const Object *self, const char *substr);
ssize_t         _string_findstr(const Object *self, const char *substr);

typedef ssize_t (*t_str_find)(const Object *self, int c);
ssize_t         _string_find(const Object *self, int c);
ssize_t         _string_lfind(const Object *self, int c);

typedef t_bool  (*t_str_match)(const Object *self, const char *compare);
t_bool          _string_match(const Object *self, const char *compare);

typedef size_t  (*t_str_nmatch)(const Object *self, const char *compare);
size_t          _string_nmatch(const Object *self, const char *compare);

typedef Object  *(*t_str_split)(const Object *self, const Class *type, const char *sep);
Object          *_string_split(const Object *self, const Class *type, const char *sep);

typedef struct
{
    Container       base;
    t_dup           dup;
    t_str_findstr   find_str;
    t_str_find      find;
    t_str_find      lfind;
    t_str_match     match;
    t_str_nmatch    nmatch;
    t_str_split     split;
} String;

t_bool  _string_insert_at(Object *self, void *data, ssize_t pos);
t_bool  _string_delete_at(Object *self, ssize_t pos);
t_bool  _string_erase(Object *self);
void    _string_affect(Object *self, void *data);
Object  *_string_front(const Object *self);
Object  *_string_back(const Object *self);
Object  *_string_at(const Object *self, ssize_t pos);

void    string_basic_print(ssize_t i, const t_data *elem, const char *prefix);

extern Class *_string;

#endif /* !STRINGSDEF_H_ */
