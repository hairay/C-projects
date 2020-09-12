#ifndef ARCH_X86_H
#define ARCH_X86_H 1

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __STDC__
typedef signed char Sint8;
#else
typedef char Sint8;
#endif
typedef unsigned char Uint8;

#define MINSINT8  ((char) 0x80)
#define MAXSINT8  ((char) 0x7f)
#define MINUINT8  ((unsigned char) 0x00)
#if !defined(_MSC_VER)
#define MAXUINT8  ((unsigned char) 0xff)
#endif

// 16 bit 
typedef short int Sint16;
typedef unsigned short int Uint16;

#define MINSINT16  ((short int) 0x8000)
#define MAXSINT16  ((short int) 0x7fff)
#define MINUINT16  ((unsigned short int) 0x0000)
#if !defined(_MSC_VER)
#define MAXUINT16  ((unsigned short int) 0xffff)
#endif

// 32 bit
typedef int Sint32;
typedef unsigned int Uint32;
typedef unsigned int UNSIGNED;

#define MINSINT32  ((long int) 0x80000000)
#define MAXSINT32  ((long int) 0x7fffffff)
#define MINUINT32  ((unsigned long int) 0x00000000)
#if !defined(_MSC_VER)
#define MAXUINT32  ((unsigned long int) 0xffffffff)
#endif

//
// 64 bit types
//
#if defined(_MSC_VER) // MICROSOFT VC compiler

typedef __int64 Sint64;
typedef unsigned __int64 Uint64;

#define MINSINT64  ((__int64) 0x8000000000000000)
#define MAXSINT64  ((__int64) 0x7fffffffffffffff)
#define MINUINT64  ((unsigned __int64) 0x0000000000000000)

#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || defined(_M_X64)
// this is a 64 bit cpu build, where ints are 32, but ptrs and longs are 64
#define ARCH_PTR_BITS 64
#define ARCH_LONG_BITS 64
#define ARCH_INT_BITS 32
#else
// this is a 32 bit cpu build with 32 bit ptrs
#define ARCH_PTR_BITS 32
#define ARCH_LONG_BITS 32
#define ARCH_INT_BITS 32
#endif

// hack these into shape
//
//#define snprintf _snprintf
//#define vsnprintf _vsnprintf
#define strcasecmp stricmp
#define strncasecmp strincmp
#define stricmp _stricmp

// no inline for older msvc
//
#ifdef _MSC_VER
#define inline __inline
#endif

#else

// Gnu compiler, or ARM maybe? 
//
typedef long long Sint64;
typedef unsigned long long Uint64;

#define MINSINT64  ((long long) 0x8000000000000000)
#define MAXSINT64  ((long long) 0x7fffffffffffffff)
#define MINUINT64  ((unsigned long long) 0x0000000000000000)
#define MAXUINT64  ((unsigned long long) 0xffffffffffffffff)

#ifdef __GNUC__
#if defined(__LP64__)||defined(__x86_64__)
#define ARCH_PTR_BITS 64
#define ARCH_LONG_BITS 64
#define ARCH_INT_BITS 32
#else
#define ARCH_PTR_BITS 32
#define ARCH_LONG_BITS 32
#define ARCH_INT_BITS 32
#endif
#else
 <! set those arch sizes right !>
#endif
#endif
     
// deprecated, make this go away
//
typedef int Boolean;
typedef int Bool;
typedef int BOOL;
#define LOCAL static

#define BIT8(n)   ((Uint8) (1 << n))
#define BIT16(n)  ((Uint16) (1 << n))
#define BIT32(n)  ((Uint32) (1 << n))
	
#if !defined(TRUE)
#define TRUE (1)
#endif

#if !defined(FALSE)
#define FALSE (0)
#endif

//  Floating Point Types
//
typedef float Float32;

#define MINFLOAT32 ((float) 0xffffffff) 
#define MAXFLOAT32 ((float) 0x7fffffff) 

typedef double Float64;

#define MINFLOAT64 ((float) 0xffffffffffffffff) 
#define MAXFLOAT64 ((float) 0x7fffffffffffffff) 

// Integer type that can hold a pointer type
//
#if (ARCH_PTR_BITS == 64)
typedef Uint64 mmsize, physaddr_t, physsize_t;
#else
typedef Uint32 mmsize, physaddr_t, physsize_t;
#endif

#endif

