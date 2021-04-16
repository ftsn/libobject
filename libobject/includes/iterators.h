#ifndef ITERATORS_H_
# define ITERATORS_H_

# include "object.h"
# include "types.h"


typedef enum
{
    END = -1,
    BEGIN
} t_it_type;

typedef struct s_iterator       Iterator;
typedef struct s_iterator       ForwardIterator;
typedef struct s_iterator       BidirectionalIterator;
typedef struct s_ra_iterator    RandomAccessIterator;

typedef t_bool  (*t_it_compare)(Iterator *it1, Iterator *it2);
t_bool          _it_equals(Iterator *it1, Iterator *it2);

typedef t_bool  (*t_it_move)(Iterator *it);
t_bool          _list_it_previous(Iterator *it);
t_bool          _list_it_next(Iterator *it);
t_bool          _array_ra_it_previous(Iterator *it);
t_bool          _array_ra_it_next(Iterator *it);
t_bool          _string_ra_it_previous(Iterator *it);
t_bool          _string_ra_it_next(Iterator *it);
t_bool          _dict_bidirectional_it_previous(Iterator *it);
t_bool          _dict_bidirectional_it_next(Iterator *it);

typedef Object  *(*t_it_data_access)(Iterator *it);
Object          *_list_it_dereference(Iterator *it);
Object          *_ra_it_dereference(Iterator *it);
Object          *_dict_bidirectional_it_dereference(Iterator *it);

typedef t_bool  (*t_ra_it_compare)(RandomAccessIterator *it1, RandomAccessIterator *it2);
t_bool          _ra_it_lt(RandomAccessIterator *it1, RandomAccessIterator *it2);
t_bool          _ra_it_gt(RandomAccessIterator *it1, RandomAccessIterator *it2);

typedef t_bool  (*t_ra_it_move)(RandomAccessIterator *it, ssize_t idx);
t_bool          _array_ra_it_jump(RandomAccessIterator *it, ssize_t idx);
t_bool          _string_ra_it_jump(RandomAccessIterator *it, ssize_t idx);

typedef Object  *(*t_ra_it_data_access)(RandomAccessIterator *it, ssize_t idx);
Object          *_array_ra_it_at(RandomAccessIterator *it, ssize_t idx);
Object          *_string_ra_it_at(RandomAccessIterator *it, ssize_t idx);

struct s_iterator {
    Class               base;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
    size_t              reached_the_end;
    size_t              reached_the_beginning;
};

typedef struct s_dict_bidirectional_iterator {
    Iterator    base;
    ssize_t     internal_idx;
} DictBidirectionalIterator;

struct s_ra_iterator {
    Iterator            base;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
};

#define SPL_LIST_IT _spl_list_it
#define DBL_LIST_IT _dbl_list_it
#define ARRAY_IT    _array_it
#define STRING_IT   _string_it
#define DICT_IT     _dict_it

// Constructor declaration
ctor_declaration(Object *, SPL_LIST_IT, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DBL_LIST_IT, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, ARRAY_IT, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, STRING_IT, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DICT_IT, Object *class; Object *iterable; t_it_type start_pos;)

Object  *generate_it(Object *self, t_it_type type);

extern Class *ARRAY_IT;
extern Class *STRING_IT;
extern Class *DICT_IT;
extern Class *SPL_LIST_IT;
extern Class *DBL_LIST_IT;

#endif /* !ITERATORS_H_ */
