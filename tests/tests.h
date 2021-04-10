#ifndef TESTS_H_
# define TESTS_H_

# include <cmocka.h>
# include "lists.h"

# define MAX_TEST_PER_GROUP  128

// Array
# define ARRAY_LOOKUP(ctn, pos)         (((t_data **)ctn->contained)[(pos)])
# define ARRAY_DATA_LOOKUP(ctn, pos)    (((t_data **)ctn->contained)[(pos)]->data)

// List
# define NODE_TO_DATA(node)             (((t_data *)((t_list_data *)node)->data)->data)
# define LOOP_OVER_ALL_KINDS(x)                                             \
do {                                                                        \
    Class *kinds[] = {_spl_list, _spl_clist, _dbl_list, _dbl_clist, NULL};  \
    for (size_t kinds_idx = 0; kinds[kinds_idx]; ++kinds_idx)               \
    {                                                                       \
        x                                                                   \
    }                                                                       \
} while (0)

// Dict
# define DICT_PAIR_LOOKUP(ctn, key)     (((Dict *)ctn)->get_by_key(ctn, (unsigned char *)key))
# define DICT_DATA_LOOKUP(ctn, key)     (((t_data *)((Dict *)ctn)->get_by_key(ctn, (unsigned char *)key))->data)

extern const struct CMUnitTest string_tests[MAX_TEST_PER_GROUP];
extern const struct CMUnitTest array_tests[MAX_TEST_PER_GROUP];
extern const struct CMUnitTest list_tests[MAX_TEST_PER_GROUP];
extern const struct CMUnitTest dict_tests[MAX_TEST_PER_GROUP];

#endif /* !TESTS_H_ */