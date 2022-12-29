#pragma once

#if defined(_MSC_VER)
    #define BFBMP_EXPORT __declspec(dllexport)
    #define BFBMP_IMPORT __declspec(dllimport)
    #pragma warning(disable : 4251)
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define BFBMP_EXPORT __attribute__ ((visibility ("default")))
    #define BFBMP_IMPORT __attribute__ ((visibility ("default")))
#endif

#if defined(BFBMP_SHARED)
    #if defined(BFBMP_EXPORTS)
        #define  BFBMP_API BFBMP_EXPORT
    #else
        #define BFBMP_API BFBMP_IMPORT
    #endif
#elif defined(BFBMP_STATIC)
    #define BFBMP_API
#endif

#define BFBMP_VERSION_MAJOR 0
#define BFBMP_VERSION_MINOR 4
#define BFBMP_VERSION ((BFBMP_VERSION_MAJOR << 16 & 0xFF00) | (BFBMP_VERSION_MINOR & 0xFF))

#define BFBMP_BEGIN namespace bfbmp {
#define BFBMP_END }

#define BFBMP_NODISCARD [[nodiscard]]