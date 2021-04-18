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
#define Iterator_fields                             \
        t_it_compare        equals;                 \
                                                    \
        t_it_move           previous;               \
        t_it_move           next;                   \
                                                    \
        t_it_data_access    dereference;            \
                                                    \
        Object              *iterated_obj;          \
        Object              *cur;                   \
        ssize_t             it_idx;                 \
        size_t              reached_the_end;        \
        size_t              reached_the_beginning;
forward_declared_class_declaration(Iterator)

// Forward iterators
#define ForwardIterator_fields  \
        Iterator_fields
forward_declared_class_declaration(ForwardIterator)

#define SplListFwdIterator_fields   \
        ForwardIterator_fields
class_declaration(SplListFwdIterator)

// BiDirectional iterators
#define BidirectionalIterator_fields    \
        Iterator_fields
forward_declared_class_declaration(BidirectionalIterator)

#define DblListBidirectionalIterator_fields \
        BidirectionalIterator_fields
class_declaration(DblListBidirectionalIterator)

#define DictBidirectionalIterator_fields    \
        BidirectionalIterator_fields        \
        ssize_t internal_idx;
class_declaration(DictBidirectionalIterator)

// Random access iterators
#define RandomAccessIterator_fields \
        Iterator_fields             \
                                    \
        t_ra_it_compare     lt;     \
        t_ra_it_compare     gt;     \
                                    \
        t_ra_it_move        jump;   \
                                    \
        t_ra_it_data_access at;     \
                                    \
        ssize_t             ra_idx;
forward_declared_class_declaration(RandomAccessIterator)

#define ArrayRaIterator_fields      \
        RandomAccessIterator_fields
class_declaration(ArrayRaIterator)

#define StringRaIterator_fields     \
        RandomAccessIterator_fields
class_declaration(StringRaIterator)

ctor_declaration(Object *, SplListFwdIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DblListBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, ArrayRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, StringRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DictBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)

Object  *generate_it(Object *self, t_it_type type);

#endif /* !ITERATORS_H_ */
