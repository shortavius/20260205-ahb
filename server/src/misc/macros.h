///
/// @file
///
/// @brief
/// Miscellaneous macros
///
/// @details
/// This is the header file contains the miscellaneous macros that are required
/// but not included.

#if !defined(_MISC_MACROS_H)
#define _MISC_MACROS_H

//
// Shared Includes
//
#include <stddef.h>

#undef BEGIN_C_DECLS
#undef END_C_DECLS
#if defined(__cplusplus)
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else
#  define BEGIN_C_DECLS /* empty */
#  define END_C_DECLS /* empty */
#endif

BEGIN_C_DECLS

//
// Global Macros / Definitions
//

#if !defined(container_of)
# define container_of(ptr, type, member)({              \
    const typeof(((type *)0)->member) * __mptr=(ptr);   \
    (type *)((char *)__mptr - offsetof(type, member));})
#endif

#if !defined(likely)
# define likely(x) __builtin_expect(!!(x), 1)
#endif

#if !defined(unlikely)
# define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#if !defined(MAX)
# define MAX(x, y) (x > y ? x : y)
#endif

#if !defined(MIN)
# define MIN(x, y) (x < y ? x : y)
#endif

#if !defined(COUNT_OF)
# define COUNT_OF(x)                                                     \
    ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#endif

//
// Shared Structures / Enumerations / Type Definitions
//

//
// Shared Function Prototypes
//

//
// Shared Global Variables
//

END_C_DECLS

#endif
