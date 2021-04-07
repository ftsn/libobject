#ifndef STRINGSDEF_H_
# define STRINGSDEF_H_

# include "containers.h"

// Forward declaration of the String class so we can use the type in our methods
typedef struct s_string String;

typedef t_bool  (*t_str_rand_insert)(String *self, char c, ssize_t pos);
t_bool          _string_insert_at(String *string, char c, ssize_t pos);

typedef t_bool  (*t_str_insert)(String *self, char c);
t_bool          _string_push_back(String *self, char c);

typedef t_bool  (*t_str_rand_delete)(String *self, ssize_t pos);
t_bool          _string_delete_at(String *self, ssize_t pos);

typedef t_bool  (*t_str_delete)(String *self);
t_bool          _string_erase(String *self);

typedef t_bool  (*t_str_affect)(String *self, void *data);
t_bool          _string_affect(String *self, void *data);

typedef char    *(*t_str_access)(const String *self);
char            *_string_front(const String *self);
char            *_string_back(const String *self);

typedef char    *(*t_str_rand_access)(const String *self, ssize_t pos);
char            *_string_at(const String *self, ssize_t pos);

typedef void    (*t_str_dump)(const String *self, const char *title);
void            _string_print(const String *self, const char *title);

typedef String  *(*t_dup)(const String *self);
String          *_string_dup(const String *self);

typedef char    *(*t_str_findstr)(const String *self, const char *substr);
char            *_string_findstr(const String *self, const char *substr);

typedef char    *(*t_str_find)(const String *self, int c);
char            *_string_find(const String *self, int c);
char            *_string_lfind(const String *self, int c);

typedef t_bool  (*t_str_match)(const String *self, const char *compare);
t_bool          _string_match(const String *self, const char *compare);

typedef size_t  (*t_str_nmatch)(const String *self, const char *compare);
size_t          _string_nmatch(const String *self, const char *compare);

typedef Object  *(*t_str_split)(const String *self, const Class *type, const char *sep);
Object          *_string_split(const String *self, const Class *type, const char *sep);

typedef char    *(*t_str_sub)(const String *self, ssize_t begin, ssize_t len);
char            *_string_sub(const String *self, ssize_t begin, ssize_t len);

struct s_string
{
    Class               base;
    char                *contained;
    ssize_t             contained_size;

    t_get_data          cstr;

    t_size              size;
    t_empty             empty;

    t_str_rand_insert   insert_at;
    t_str_insert        push_back;
    t_str_rand_delete   delete_at;
    t_str_delete        erase;
    t_str_affect        affect;

    t_str_access        front;
    t_str_access        back;
    t_str_rand_access   at;

    t_str_dump          dump;
    
    t_dup               dup;
    t_str_findstr       find_str;
    t_str_find          find;
    t_str_find          lfind;
    t_str_match         match;
    t_str_nmatch        nmatch;
    t_str_split         split;
    t_str_sub           sub;
};

extern Class *_string;

#endif /* !STRINGSDEF_H_ */
