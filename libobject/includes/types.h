#ifndef TYPES_H_
# define TYPES_H_

typedef void Object;

typedef char    t_bool;
# define TRUE   1
# define FALSE  (!TRUE)

typedef enum	e_type
{
    /* Primary types */
    _TYPE_PRIMARY_BEGIN = 0,
    TYPE_CHAR,
    TYPE_UCHAR,
    TYPE_CSTRING,
    TYPE_INT,
    TYPE_UINT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_INT64,
    TYPE_UINT64,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_PTR,

    /* Bool type defined below */
    TYPE_BOOL,
    _TYPE_PRIMARY_END,

    /* Classes types */
    _TYPE_CONTAINER_BEGIN,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_DICT,
    TYPE_LINKED_LIST,
    TYPE_CIRCULAR_LINKED_LIST,
    TYPE_DOUBLY_LINKED_LIST,
    TYPE_CIRCULAR_DOUBLY_LINKED_LIST,
    TYPE_ARRAY_ITERATOR,
    TYPE_STRING_ITERATOR,
    TYPE_DICT_ITERATOR,
    TYPE_LIST_ITERATOR,
    _TYPE_CONTAINER_END
} t_type;

typedef struct
{
    t_type    type;
    void      *data;
} t_data;

static inline t_bool    is_of_type(Object *tested, t_type type)
{
  return (*(t_type *)tested == type ? TRUE : FALSE);
}

static inline t_bool    is_primary(Object *tested)
{
  return (*(t_type *)tested > _TYPE_PRIMARY_BEGIN && *(t_type *)tested < _TYPE_PRIMARY_END ? TRUE : FALSE);
}

static inline t_bool    is_container(Object *tested)
{
  return (*(t_type *)tested > _TYPE_CONTAINER_BEGIN && *(t_type *)tested < _TYPE_CONTAINER_END ? TRUE : FALSE);
}

static inline t_bool    is_list(Object *tested)
{
  return (*(t_type *)tested >= TYPE_LINKED_LIST && *(t_type *)tested <= TYPE_CIRCULAR_DOUBLY_LINKED_LIST ? TRUE : FALSE);
}

t_data  *raw_data_to_typed(void *raw, t_type type);
void    free_typed_array(t_data **array);
t_data  **init_typed_array(size_t nmemb);
t_data  **cstrings_to_data_array(char **array);

#endif /* !TYPES_H_ */
