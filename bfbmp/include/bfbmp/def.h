#pragma once
#include <stdint.h>
#include <stddef.h>


#if !defined(BFBMP_PUBLIC)
#if !defined(__cplusplus)
#define BFBMP_PUBLIC extern
#else
#define BFBMP_PUBLIC extern "C"
#endif
#endif

#if !defined(BFBMP_PRIVATE)
#define BFBMP_PRIVATE static
#endif


#if defined(_MSC_VER)
#define BFBMP_EXPORT __declspec(dllexport)
#define BFBMP_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#define BFBMP_EXPORT __attribute__((visibility( "default" )))
#define BFBMP_IMPORT __attribute__((visibility( "default" )))
#else
#error "Couldn't compile libqrc on this platform!"
#endif

#if !defined(BFBMP_API)
#if defined(BFBMP_STATIC)
#define BFBMP_API
#else
#if defined(BFBMP_SHARED)
#define BFBMP_API BFBMP_EXPORT
#else
#define BFBMP_API BFBMP_IMPORT
#endif
#endif
#else
#define BFBMP_API
#endif

#define BFBMP_TRUE  1
#define BFBMP_FALSE 0

#if !defined(BFBMP_CHECK)
#if defined(BFBMP_USE_ASSERTION)
#include "assert.h"
#define BFBMP_CHECK(condition) assert((condition))
#else
#define BFBMP_CHECK(condition) if(!(condition)) return BFBMP_FALSE
#endif
#endif

#define BFBMP_VERSION_MAJOR 1
#define BFBMP_VERSION_MINOR 0
#define BFBMP_VERSION ((BFBMP_VERSION_MAJOR << 16 & 0xFF00) | (BFBMP_VERSION_MINOR & 0xFF))
#define BFBMP_ZERO 0
#define BFBMP_2UC(str) (const uint8_t*)(str)
#define BFBMP_2C(str) (const char*)(str)