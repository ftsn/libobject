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

#define ITERATOR            iterator_blueprint
#define FWD_IT              fwd_it_blueprint
#define BIDIRECTIONAL_IT    bidirectional_it_blueprint
#define RA_IT               ra_it_blueprint

#define SPL_LIST_IT         spl_list_it_blueprint
#define DBL_LIST_IT         dbl_list_it_blueprint
#define ARRAY_IT            array_it_blueprint
#define STRING_IT           string_it_blueprint
#define DICT_IT             dict_it_blueprint

// Constructor declaration
forward_declared_class_declaration(Iterator, ITERATOR,
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
)

// Forward iterators
forward_declared_class_declaration(ForwardIterator, FWD_IT,
    Iterator            base;
)
class_declaration(SplListFwdIterator, SPL_LIST_IT,
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
)

// BiDirectional iterators
forward_declared_class_declaration(BidirectionalIterator, BIDIRECTIONAL_IT,
    Iterator            base;
)
class_declaration(DblListBidirectionalIterator, DBL_LIST_IT,
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
)
class_declaration(DictBidirectionalIterator, DICT_IT,
    Iterator    base;
    ssize_t     internal_idx;
)
forward_declared_class_declaration(RandomAccessIterator, RA_IT,
    Iterator            base;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)
class_declaration(ArrayRaIterator, ARRAY_IT,
    Iterator            base;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)
class_declaration(StringRaIterator, STRING_IT,
    Iterator            base;

    t_ra_it_compare     lt;
    t_ra_it_compare     gt;

    t_ra_it_move        jump;

    t_ra_it_data_access at;

    ssize_t             ra_idx;
)

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
