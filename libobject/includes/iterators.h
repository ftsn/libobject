#ifndef ITERATORS_H_
# define ITERATORS_H_

# include "object.h"
# include "types.h"


typedef enum
{
    END = -1,
    BEGIN
} t_it_type;

forward_class_declaration(Iterator)
forward_class_declaration(ForwardIterator)
forward_class_declaration(BidirectionalIterator)
forward_class_declaration(RandomAccessIterator)

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

// Constructor declaration
forward_declared_class_declaration(Iterator,
    Class;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
    size_t              reached_the_end;
    size_t              reached_the_beginning;
)

// Forward iterators
forward_declared_class_declaration(ForwardIterator,
    Iterator;
)
class_declaration(SplListFwdIterator,
    Class;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
    size_t              reached_the_end;
    size_t              reached_the_beginning;
)

// BiDirectional iterators
forward_declared_class_declaration(BidirectionalIterator,
    Iterator;
)
class_declaration(DblListBidirectionalIterator,
    Class;

    t_it_compare        equals;

    t_it_move           previous;
    t_it_move           next;

    t_it_data_access    dereference;

    Object              *iterated_obj;
    Object              *cur;
    ssize_t             it_idx;
    size_t              reached_the_end;
    size_t              reached_the_beginning;
)
class_declaration(DictBidirectionalIterator,
    Iterator    base;
    ssize_t     internal_idx;
)
forward_declared_class_declaration(RandomAccessIterator,
    Iterator;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)
class_declaration(ArrayRaIterator,
    Iterator;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)
class_declaration(StringRaIterator,
    Iterator;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)

ctor_declaration(Object *, SplListFwdIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DblListBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, ArrayRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, StringRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DictBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)

Object  *generate_it(Object *self, t_it_type type);

#endif /* !ITERATORS_H_ */
