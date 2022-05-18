/**
 * \file std_types.h
 *
 * \brief Standard type definitions.
 */

#ifndef STD_TYPES_H_FILE
#define STD_TYPES_H_FILE

#ifndef TRUE
#define TRUE (1U)
#endif

#ifndef FALSE
#define FALSE (0U)
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void *) 0U)
#endif

#ifndef NULL
#define NULL NULL_PTR
#endif

#include <stdint.h>

typedef unsigned char boolean;
typedef uint8_t       uint8;
typedef uint16_t      uint16;
typedef int8_t        sint8;
typedef int16_t       sint16;

#if defined(__x86_64)
// important (!) this framework is also compiled for 64-bit
// architectures where long-types are different (!)
#include <stdint.h>
typedef int32_t  sint32;
typedef uint32_t uint32;
typedef int64_t  sint64;
typedef uint64_t uint64;
#else
typedef signed long        sint32;
typedef unsigned long      uint32;
typedef signed long long   sint64;
typedef unsigned long long uint64;
#endif

typedef float  float32;
typedef double float64;

/* not using t_ prefix because this is not a user-type, it's a platform type */
typedef uint8 std_return_type;

#ifndef STD_OK
#define STD_OK 0U
#endif

#ifndef STD_NOT_OK
#define STD_NOT_OK 1U
#endif

#ifndef STD_BUSY
#define STD_BUSY 2U
#endif

#ifndef STD_OVFL
#define STD_OVFL 3U
#endif

#ifndef STD_HIGH
#define STD_HIGH 1U
#endif

#ifndef STD_LOW
#define STD_LOW 0U
#endif

#ifndef STD_ON
#define STD_ON 1U
#endif

#ifndef STD_OFF
#define STD_OFF 0U
#endif

#endif /* STD_TYPES_H_FILE */
