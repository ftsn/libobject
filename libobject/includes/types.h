#ifndef TYPES_H_
# define TYPES_H_

typedef void    Object;

typedef char    t_bool;
# define TRUE   1
# define FALSE  (!TRUE)

typedef enum e_type
{
    /* Primary types */
    _TYPE_PRIMARY_BEGIN = 0,
    TYPE_CHAR,
    TYPE_UCHAR,
    TYPE_CSTRING,
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_INT,
    TYPE_UINT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_INT32,
    TYPE_UINT32,
    TYPE_INT64,
    TYPE_UINT64,
    TYPE_LONGLONG,
    TYPE_ULONGLONG,
    TYPE_INTMAX,
    TYPE_UINTMAX,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_PTR,
    TYPE_PTRDIFF,

    /* Bool type defined below */
    TYPE_BOOL,
    _TYPE_PRIMARY_END,

    /* Classes types */

    /* Containers */
    _TYPE_CONTAINER_BEGIN,
    TYPE_ARRAY,
    TYPE_DICT,
    TYPE_LINKED_LIST,
    TYPE_CIRCULAR_LINKED_LIST,
    TYPE_DOUBLY_LINKED_LIST,
    TYPE_CIRCULAR_DOUBLY_LINKED_LIST,
    _TYPE_CONTAINER_END,

    /* Iterators */
    _TYPE_ITERATOR_BEGIN,
    TYPE_ARRAY_ITERATOR,
    TYPE_STRING_ITERATOR,
    TYPE_SPL_LIST_ITERATOR,
    TYPE_DBL_LIST_ITERATOR,
    TYPE_DICT_ITERATOR,
    _TYPE_ITERATOR_END,

    /* Other classes */
    TYPE_STRING,
    TYPE_PAIR,

    /* Should always be at the end of the enum */
    LOBJ_LAST_TYPE,
} t_type;

typedef struct
{
    t_type  type;
    void    *data;
} t_data;

static inline t_bool    is_of_type(const Object *tested, t_type type)
{
    return (*(t_type *)tested == type ? TRUE : FALSE);
}

static inline t_bool    is_primary(const Object *tested)
{
    return (*(t_type *)tested > _TYPE_PRIMARY_BEGIN && *(t_type *)tested < _TYPE_PRIMARY_END ? TRUE : FALSE);
}

static inline t_bool    is_container(Object *tested)
{
    return (*(t_type *)tested > _TYPE_CONTAINER_BEGIN && *(t_type *)tested < _TYPE_CONTAINER_END ? TRUE : FALSE);
}

static inline t_bool    is_list(const Object *tested)
{
    return (*(t_type *)tested >= TYPE_LINKED_LIST && *(t_type *)tested <= TYPE_CIRCULAR_DOUBLY_LINKED_LIST ? TRUE : FALSE);
}

static inline t_bool    is_spl_list(const Object *tested)
{
    return (*(t_type *)tested == TYPE_LINKED_LIST || *(t_type *)tested == TYPE_CIRCULAR_LINKED_LIST ? TRUE : FALSE);
}

static inline t_bool    is_dbl_list(const Object *tested)
{
    return (*(t_type *)tested == TYPE_DOUBLY_LINKED_LIST || *(t_type *)tested == TYPE_CIRCULAR_DOUBLY_LINKED_LIST ? TRUE : FALSE);
}

static inline t_bool    is_clist(const Object *tested)
{
    return (*(t_type *)tested == TYPE_CIRCULAR_LINKED_LIST || *(t_type *)tested == TYPE_CIRCULAR_DOUBLY_LINKED_LIST ? TRUE : FALSE);
}

static inline t_bool    is_iterator(const Object *tested)
{
    return (*(t_type *)tested >= _TYPE_ITERATOR_BEGIN && *(t_type *)tested <= _TYPE_ITERATOR_END ? TRUE : FALSE);
}

t_data  *raw_data_to_typed(void *raw, t_type type);
void    free_typed_array(t_data **array);
t_data  **init_typed_array(size_t nmemb);
t_data  **cstrings_to_data_array(char **array);

#endif /* !TYPES_H_ */
