#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "arrays.h"
#include "lists.h"
#include "stringsdef.h"
#include "iterators.h"
#include "dicts.h"
#include "variadic.h"

struct a
{
  int a;
  int b;
  int c;
  char *s;
  char ch;
  double d;
  double e;
};

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, s, x, ...) what(s, x)
#define FOR_EACH_2(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_1(what, s, __VA_ARGS__);
#define FOR_EACH_3(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_2(what, s, __VA_ARGS__);
#define FOR_EACH_4(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_3(what, s, __VA_ARGS__);
#define FOR_EACH_5(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_4(what, s, __VA_ARGS__);
#define FOR_EACH_6(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_5(what, s, __VA_ARGS__);
#define FOR_EACH_7(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_6(what, s, __VA_ARGS__);
#define FOR_EACH_8(what, s, x, ...)\
  what(s, x);\
  FOR_EACH_7(what, s, __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__) 
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N 
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, s, x, ...) CONCATENATE(FOR_EACH_, N)(what, s, x, __VA_ARGS__)
#define FOR_EACH(what, s, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, s, x, __VA_ARGS__)

#define PRN_STRUCT_OFFSETS_(s, structure, field) printf(STRINGIZE(structure)":"STRINGIZE(field)" - offset = %zd [%s]\n", offsetof(structure, field), s);
#define PRN_STRUCT_OFFSETS(s, field) PRN_STRUCT_OFFSETS_(s, struct a, field)

int main(int argc, char *argv[])
{
    FOR_EACH(PRN_STRUCT_OFFSETS, "HELLO", a, b, c, s, ch, d);
    printf("\n");

    (void)argc;
    (void)argv;
    return (0);
}
