#ifndef ITERATORS_H_
# define ITERATORS_H_

# include "object.h"
# include "types.h"


typedef enum
{
    END = -1,
    BEGIN
} t_it_type;

typedef struct s_iterator               Iterator;
typedef struct s_forward_iterator       ForwardIterator;
typedef struct s_bidirectional_iterator BidirectionalIterator; // For now Iterator == Bidirectional iterator
typedef struct s_ra_iterator            RandomAccessIterator;

typedef t_bool  (*t_it_compare)(Iterator *it1, Iterator *it2);
t_bool          _it_equals(Iterator *it1, Iterator *it2); // => OK

typedef t_bool  (*t_it_move)(Iterator *it);
t_bool          _list_it_previous(Iterator *it); // OK
t_bool          _list_it_next(Iterator *it); // OK
t_bool          _ra_it_previous(Iterator *it); // OK
t_bool          _ra_it_next(Iterator *it); // OK

typedef Object  *(*t_it_data_access)(Iterator *it);
Object          *_list_it_dereference(Iterator *it); // OK
Object          *_ra_it_dereference(Iterator *it); // OK

typedef t_bool  (*t_ra_it_compare)(RandomAccessIterator *it1, RandomAccessIterator *it2);
t_bool          _ra_it_lt(RandomAccessIterator *it1, RandomAccessIterator *it2); // OK
t_bool          _ra_it_gt(RandomAccessIterator *it1, RandomAccessIterator *it2); // OK

typedef t_bool  (*t_ra_it_move)(RandomAccessIterator *it, ssize_t idx);
t_bool          _array_ra_it_jump(RandomAccessIterator *it, ssize_t idx); // OK

typedef Object  *(*t_ra_it_data_access)(RandomAccessIterator *it, ssize_t idx);
Object          *_ra_it_at(RandomAccessIterator *it, ssize_t idx); // OK

struct s_iterator {
    Class               base;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
};

struct s_bidirectional_iterator {
    Class               base;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
};

struct s_forward_iterator {
    Class               base;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
};



struct s_ra_iterator {
    Iterator            base;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
};

Object  *generate_it(const Object *self, t_it_type type);

extern Class *_array_ra_it;
extern Class *_string_ra_it;
extern Class *_dict_ra_it;
extern Class *_spl_list_forward_it;
extern Class *_dbl_list_bidirectional_it;

#endif /* !ITERATORS_H_ */
