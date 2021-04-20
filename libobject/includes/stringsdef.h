#ifndef STRINGSDEF_H_
# define STRINGSDEF_H_

# include "containers.h"

// Forward declaration of the String class so we can use the type in our methods
forward_class_declaration(String)

typedef Object  *(*t_str_get_data)(const String *self);
Object          *string_data(const String *self);

typedef ssize_t (*t_str_size)(const String *self);
ssize_t         string_size(const String *self);

typedef t_bool  (*t_str_empty)(const String *self);
t_bool          string_empty(const String *self);

typedef t_bool  (*t_str_rand_insert)(String *self, char c, ssize_t pos);
t_bool          string_insert_at(String *string, char c, ssize_t pos);

typedef t_bool  (*t_str_insert)(String *self, char c);
t_bool          string_push_back(String *self, char c);

typedef t_bool  (*t_str_rand_delete)(String *self, ssize_t pos);
t_bool          string_delete_at(String *self, ssize_t pos);

typedef t_bool  (*t_str_delete)(String *self);
t_bool          string_erase(String *self);

typedef t_bool  (*t_str_affect)(String *self, void *data);
t_bool          string_affect(String *self, void *data);

typedef char    *(*t_str_access)(const String *self);
char            *string_front(const String *self);
char            *string_back(const String *self);

typedef char    *(*t_str_rand_access)(const String *self, ssize_t pos);
char            *string_at(const String *self, ssize_t pos);

typedef void    (*t_str_dump)(const String *self, const char *title);
void            string_print(const String *self, const char *title);

typedef Object  *(*t_str_it_create)(Object *self);
Object          *string_begin(Object *self);
Object          *string_end(Object *self);

typedef char    *(*t_str_findstr)(const String *self, const char *substr);
char            *string_findstr(const String *self, const char *substr);

typedef char    *(*t_str_find)(const String *self, int c);
char            *string_find(const String *self, int c);
char            *string_lfind(const String *self, int c);

typedef t_bool  (*t_str_match)(const String *self, const char *compare);
t_bool          string_match(const String *self, const char *compare);

typedef ssize_t (*t_str_nmatch)(const String *self, const char *compare);
ssize_t         string_nmatch(const String *self, const char *compare);

typedef Object  *(*t_str_split)(const String *self, const Class *type, const char *sep);
Object          *string_split(const String *self, const Class *type, const char *sep);

typedef char    *(*t_str_sub)(const String *self, ssize_t begin, ssize_t len);
char            *string_sub(const String *self, ssize_t begin, ssize_t len);

// Class and constructor declaration
#define String_fields                       \
        char                *contained;     \
        ssize_t             contained_size;
#define String_vtable_fields                \
        t_str_get_data      cstr;           \
                                            \
        t_str_size          size;           \
        t_str_empty         empty;          \
                                            \
        t_str_rand_insert   insert_at;      \
        t_str_insert        push_back;      \
        t_str_rand_delete   delete_at;      \
        t_str_delete        erase;          \
        t_str_affect        affect;         \
                                            \
        t_str_access        front;          \
        t_str_access        back;           \
        t_str_rand_access   at;             \
                                            \
        t_str_dump          dump;           \
                                            \
        t_str_it_create     begin;          \
        t_str_it_create     end;            \
                                            \
        t_str_findstr       find_str;       \
        t_str_find          find;           \
        t_str_find          lfind;          \
        t_str_match         match;          \
        t_str_nmatch        nmatch;         \
        t_str_split         split;          \
        t_str_sub           sub;
#define String_definition               \
        .contained = NULL,              \
        .contained_size = 0
#define String_vtable_definition        \
        .cstr = string_data,            \
        .size = string_size,            \
        .empty = string_empty,          \
        .insert_at = string_insert_at,  \
        .push_back = string_push_back,  \
        .delete_at = string_delete_at,  \
        .erase = string_erase,          \
        .affect = string_affect,        \
        .front = string_front,          \
        .back = string_back,            \
        .at = string_at,                \
        .dump = string_print,           \
        .begin = string_begin,          \
        .end = string_end,              \
        .find_str = string_findstr,     \
        .find = string_find,            \
        .lfind = string_lfind,          \
        .match = string_match,          \
        .nmatch = string_nmatch,        \
        .split = string_split,          \
        .sub = string_sub

_forward_declared_class_declaration(String)
ctor_declaration(Object *, String, Object *class; char *to_copy; ssize_t copy_amount;)

#endif /* !STRINGSDEF_H_ */
