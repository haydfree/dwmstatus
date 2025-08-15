#ifndef DEFINES_H
#define DEFINES_H

#define GUARD_OOB(val, min, max)                do { if ((val) < (min) || (val) > (max)) { goto cleanup; } } while (0)
#define GUARD_NULL(ptr)                         do { if (!(ptr)) { goto cleanup; } } while (0)
#define GUARD_TRUE(expr)                        do { if ((expr)) { goto cleanup; } } while (0)
#define GUARD_FALSE(expr)                       do { if (!(expr)) { goto cleanup; } } while (0)
#define GUARD_POS(val)                          do { if ((val) > 0) { goto cleanup; } } while (0)
#define GUARD_NEG(val)                          do { if ((val) < 0) { goto cleanup; } } while (0)
#define GUARD_ZERO(val)                         do { if ((val) == 0) { goto cleanup; } } while (0)
#define GUARD_NON_POS(val)                      do { if ((val) <= 0) { goto cleanup; } } while (0)
#define GUARD_NON_NEG(val)                      do { if ((val) >= 0) { goto cleanup; } } while (0)
#define GUARD_NON_ZERO(val)                     do { if ((val) != 0) { goto cleanup; } } while (0)
#define GUARD_ERROR(expr)                       GUARD_TRUE(expr)
#define GUARD_EQUAL(val1, val2)					do { if (val1 == val2) { goto cleanup; } } while (0)
#define GUARD_NOT_EQUAL(val1, val2)				do { if (val1 != val2) { goto cleanup; } } while (0)

#define ZERO_MEM(ptr, size)						memset(ptr, 0, size)	
#define FREE(ptr)								if ((ptr)) { free(ptr); ptr = NULL; }

typedef unsigned char u8; 
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8; 
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef int b32;
typedef char b8;

#define STATIC_ASSERT static_assert

STATIC_ASSERT(sizeof(u8) == 1, "expected u8 to be 1 byte\n");
STATIC_ASSERT(sizeof(u16) == 2, "expected u16 to be 2 bytes\n");
STATIC_ASSERT(sizeof(u32) == 4, "expected u32 to be 4 bytes\n");
STATIC_ASSERT(sizeof(u64) == 8, "expected u64 to be 8 bytes\n");

STATIC_ASSERT(sizeof(i8) == 1, "expected i8 to be 1 byte\n");
STATIC_ASSERT(sizeof(i16) == 2, "expected i16 to be 2 bytes\n");
STATIC_ASSERT(sizeof(i32) == 4, "expected i32 to be 4 bytes\n");
STATIC_ASSERT(sizeof(i64) == 8, "expected i64 to be 8 bytes\n");

STATIC_ASSERT(sizeof(f32) == 4, "expected f32 to be 4 bytes\n");
STATIC_ASSERT(sizeof(f64) == 8, "expected f64 to be 8 bytes\n");

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define ERROR -1

#endif
