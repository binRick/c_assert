/*
 * Out-of-box format-able assert macros
 *
 * Created May 21, 2020. leiless.
 * XXX: side-effect unsafe
 *
 * Release under BSD-2-Clause license
 */

#ifndef __ASSERTF_H
#define __ASSERTF_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>

/* Macro taken from macOS/Frameworks/Kernel/sys/cdefs.h */
#ifndef __printflike
#define __printflike(fmtarg, firstvararg) \
        __attribute__((__format__(__printf__, fmtarg, firstvararg)))
#endif

void __assertf0(int, const char *, ...) __printflike(2, 3);

/**
 * Formatted version of assert()
 *
 * @param expr  Expression to assert with
 * @param fmt   Format string when assertion failed
 * @param ...   Format string arguments
 */
void __assertf0(int expr, const char *fmt, ...)
{
    int n;
    va_list ap;

#ifndef NO_ASSERTF_H
    if (!expr) {
        va_start(ap, fmt);
        n = vfprintf(stderr, fmt, ap);
        assert(n > 0);  /* Should never fail! */
        va_end(ap);

        abort();
    }
#endif
}

#define assertf(e, fmt, ...)                                        \
    __assertf0(!!(e), "Assert (%s) failed: " fmt "  %s@%s()#%d\n",  \
                #e, ##__VA_ARGS__, basename(__BASE_FILE__), __func__, __LINE__)

#define panicf(fmt, ...)            assertf(0, fmt, ##__VA_ARGS__)
#define assert_nonnull(ptr)         assertf(ptr != NULL, "")
#define assert_null(ptr)            assertf(ptr == NULL, "")

/**
 * @param a     Left hand side
 * @param b     Right hand side
 * @param fs    Format specifier
 * @param op    Comparator
 * @param fmt   Additional format string
 * @param ...   Format string arguments
 */
#define __assert_cmp(a, b, fs, op, fmt, ...)                \
    assertf((a) op (b), "lhs: " fs " rhs: " fs " " fmt,     \
            (a), (typeof(a)) (b), ##__VA_ARGS__)

#define assert_eq(a, b, fs)             __assert_cmp(a, b, fs, ==, "")
#define assert_eqf(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, ==, fmt, ##__VA_ARGS__)

#define assert_ne(a, b, fs)             __assert_cmp(a, b, fs, !=, "")
#define assert_nef(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, !=, fmt, ##__VA_ARGS__)

#define assert_le(a, b, fs)             __assert_cmp(a, b, fs, <=, "")
#define assert_lef(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, <=, fmt, ##__VA_ARGS__)

#define assert_ge(a, b, fs)             __assert_cmp(a, b, fs, >=, "")
#define assert_gef(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, >=, fmt, ##__VA_ARGS__)

#define assert_lt(a, b, fs)             __assert_cmp(a, b, fs, <, "")
#define assert_ltf(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, <, fmt, ##__VA_ARGS__)

#define assert_gt(a, b, fs)             __assert_cmp(a, b, fs, >, "")
#define assert_gtf(a, b, fs, fmt, ...)  __assert_cmp(a, b, fs, >, fmt, ##__VA_ARGS__)

#endif

