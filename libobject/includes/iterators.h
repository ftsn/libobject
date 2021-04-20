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
t_bool          it_equals(Iterator *it1, Iterator *it2);

typedef t_bool  (*t_it_move)(Iterator *it);
t_bool          list_it_previous(Iterator *it);
t_bool          list_it_next(Iterator *it);
t_bool          array_ra_it_previous(Iterator *it);
t_bool          array_ra_it_next(Iterator *it);
t_bool          string_ra_it_previous(Iterator *it);
t_bool          string_ra_it_next(Iterator *it);
t_bool          dict_bidirectional_it_previous(Iterator *it);
t_bool          dict_bidirectional_it_next(Iterator *it);

typedef Object  *(*t_it_data_access)(Iterator *it);
Object          *list_it_dereference(Iterator *it);
Object          *ra_it_dereference(Iterator *it);
Object          *dict_bidirectional_it_dereference(Iterator *it);

typedef t_bool  (*t_ra_it_compare)(RandomAccessIterator *it1, RandomAccessIterator *it2);
t_bool          ra_it_lt(RandomAccessIterator *it1, RandomAccessIterator *it2);
t_bool          ra_it_gt(RandomAccessIterator *it1, RandomAccessIterator *it2);

typedef t_bool  (*t_ra_it_move)(RandomAccessIterator *it, ssize_t idx);
t_bool          array_ra_it_jump(RandomAccessIterator *it, ssize_t idx);
t_bool          string_ra_it_jump(RandomAccessIterator *it, ssize_t idx);

typedef Object  *(*t_ra_it_data_access)(RandomAccessIterator *it, ssize_t idx);
Object          *array_ra_it_at(RandomAccessIterator *it, ssize_t idx);
Object          *string_ra_it_at(RandomAccessIterator *it, ssize_t idx);

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
#define Iterator_definition			\
        .equals = it_equals,		\
        .previous = NULL,			\
        .next = NULL,				\
        .dereference = NULL,		\
        .iterated_obj = NULL,		\
        .cur = NULL,				\
        .it_idx = 0,				\
        .reached_the_end = 0,		\
        .reached_the_beginning = 0
forward_declared_class_declaration(Iterator)

// Forward iterators
#define ForwardIterator_fields		\
        Iterator_fields
#define ForwardIterator_definition	\
        Iterator_definition
forward_declared_class_declaration(ForwardIterator)

#define SplListFwdIterator_fields			\
        ForwardIterator_fields
#define SplListFwdIterator_definition		\
        ForwardIterator_definition,			\
		.next = list_it_next,				\
		.dereference = list_it_dereference
class_declaration(SplListFwdIterator)

// BiDirectional iterators
#define BidirectionalIterator_fields    	\
        Iterator_fields
#define BidirectionalIterator_definition	\
        Iterator_definition
forward_declared_class_declaration(BidirectionalIterator)

#define DblListBidirectionalIterator_fields 	\
        BidirectionalIterator_fields
#define DblListBidirectionalIterator_definition	\
        BidirectionalIterator_definition,		\
		.previous = list_it_previous,			\
		.next = list_it_next,					\
		.dereference = list_it_dereference
class_declaration(DblListBidirectionalIterator)

#define DictBidirectionalIterator_fields    \
        BidirectionalIterator_fields        \
        ssize_t internal_idx;
#define DictBidirectionalIterator_definition				\
        BidirectionalIterator_definition,					\
		.previous = dict_bidirectional_it_previous,		    \
		.next = dict_bidirectional_it_next,				    \
		.dereference = dict_bidirectional_it_dereference,	\
		.internal_idx = 0
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
#define RandomAccessIterator_definition		\
        Iterator_definition,				\
		.dereference = ra_it_dereference,	\
		.lt = ra_it_lt,					    \
		.gt = ra_it_gt,					    \
		.ra_idx = 0
forward_declared_class_declaration(RandomAccessIterator)

#define ArrayRaIterator_fields				\
        RandomAccessIterator_fields
#define ArrayRaIterator_definition			\
        RandomAccessIterator_definition,	\
		.previous = array_ra_it_previous,	\
		.next = array_ra_it_next,			\
		.jump = array_ra_it_jump,			\
		.at = array_ra_it_at

class_declaration(ArrayRaIterator)

#define StringRaIterator_fields				\
        RandomAccessIterator_fields
#define StringRaIterator_definition			\
        RandomAccessIterator_definition,	\
		.previous = string_ra_it_previous,	\
		.next = string_ra_it_next,			\
		.jump = string_ra_it_jump,			\
		.at = string_ra_it_at
class_declaration(StringRaIterator)

ctor_declaration(Object *, SplListFwdIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DblListBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, ArrayRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, StringRaIterator, Object *class; Object *iterable; t_it_type start_pos;)
ctor_declaration(Object *, DictBidirectionalIterator, Object *class; Object *iterable; t_it_type start_pos;)

Object  *generate_it(Object *self, t_it_type type);

#endif /* !ITERATORS_H_ */
