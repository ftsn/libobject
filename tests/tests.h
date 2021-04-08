#ifndef TESTS_H_
# define TESTS_H_

# include <cmocka.h>

# define MAX_TEST_PER_GROUP  128

extern const struct CMUnitTest string_tests[MAX_TEST_PER_GROUP];
extern const struct CMUnitTest array_tests[MAX_TEST_PER_GROUP];

#endif /* !TESTS_H_ */