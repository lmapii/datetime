/**
 * \file util.h
 *
 * \brief Utility functions
 */

#ifndef UTIL_H_FILE
#define UTIL_H_FILE

#define BIT_IS_SET(value, n)   (((value) & (1U << (n))) != 0)
#define BIT_IS_CLEAR(value, n) (!(BIT_IS_SET(value, n)))

#define BIT_HIGH(n)         ((1U << (n)))
#define BIT_SET(value, n)   ((value) |= (BIT_HIGH(n)))
#define BIT_CLEAR(value, n) ((value) &= ~(BIT_HIGH(n)))

#ifndef IS_POWER_OF_TWO
#define IS_POWER_OF_TWO(A) (((A) != 0) && ((((A) -1) & (A)) == 0))
#endif

/** macro for static assertions using sizeof as used for linux kernel */
// #define BUILD_BUG_ON(condition) (sizeof(char[1 - 2*!!(condition)]))

/*
#define BUILD_BUG_ON(condition)                                                 \
#pragma GCC diagnostic ignored "-Wunused-variable"                              \
#pragma GCC diagnostic push                                                     \
static uint8 __FILE__##__LINE__##test = (sizeof(char[1 - 2*!!(condition)]))     \
#pragma GCC diagnostic pop
*/

/* usage example: just a statement to make sure that such a check would be ok / not-ok */
/* BUILD_BUG_ON(sizeof (some_var) != sizeof (uint8)); */

/* http://www.pixelbeat.org/programming/gcc/static_assert.html */
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b)  ASSERT_CONCAT_(a, b)
/* These can't be used after statements in c89. */
#ifndef STATIC_ASSERT
#ifdef __COUNTER__
#define STATIC_ASSERT(e, m)                                            \
    ;                                                                  \
    enum                                                               \
    {                                                                  \
        ASSERT_CONCAT(static_assert_, __COUNTER__) = 1 / (int) (!!(e)) \
    }
#else
/* This can't be used twice on the same line so ensure if using in headers
 * that the headers are not included twice (by wrapping in #ifndef...#endif)
 * Note it doesn't cause an issue when used on same line of separate modules
 * compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e, m)                                       \
    ;                                                             \
    enum                                                          \
    {                                                             \
        ASSERT_CONCAT(assert_line_, __LINE__) = 1 / (int) (!!(e)) \
    }
#endif

// linux typecheck macro, only stops compilation with -WError TODO: turn on WError for production
// builds
// https://stackoverflow.com/questions/10393844/how-does-the-typecheck-macro-from-the-linux-kernel-work
#define TYPECHECK(type, x)              \
    ({                                  \
        type __dummy;                   \
        typeof(x) __dummy2;             \
        (void) (&__dummy == &__dummy2); \
        1;                              \
    })

// since protobuf does not provide constants for max_count we need to use language features
// https://stackoverflow.com/questions/3553296/sizeof-single-struct-member-in-c
#define SIZEOF_MEMBER(type, member) sizeof(((type *) 0)->member)
#endif

/* these macros are only used by unit-tests */
#ifdef TEST
#ifdef DBG_PRINT_ON
#include <stdio.h>
#define dbg_print(fmt, args...)                                                \
    do                                                                         \
    {                                                                          \
        fprintf(stdout, "[ %4d : %-20s ] - " fmt, __LINE__, __func__, ##args); \
        fflush(stderr);                                                        \
    } while (0)
#define dbg_print_plain(...) \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    } while (0)
#else
#define dbg_print(...)       ;
#define dbg_print_plain(...) ;
#endif

#ifdef TST_PRINT_ON
#include <stdio.h>
#define tst_print(fmt, args...)                                                \
    do                                                                         \
    {                                                                          \
        fprintf(stdout, "[ %4d : %-20s ] - " fmt, __LINE__, __func__, ##args); \
        fflush(stderr);                                                        \
    } while (0)
#define tst_print_plain(...) \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    } while (0)
#else
#define tst_print(...)       ;
#define tst_print_plain(...) ;
#endif
#else
#define dbg_print(...)       ;
#define dbg_print_plain(...) ;
#endif

#endif /* UTIL_H_FILE */
