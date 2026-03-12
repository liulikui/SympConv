#ifndef SYMPCONV_ENVIROMENT_H
#define SYMPCONV_ENVIROMENT_H

// OS
#if defined(_WIN32) || defined(_WIN64)
#define SYMPCONV_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define SYMPCONV_PLATFORM_APPLE
#elif defined(__ANDROID__)
#define SYMPCONV_PLATFORM_ANDROID
#elif defined(__linux__)
#define SYMPCONV_PLATFORM_LINUX
#else
#define SYMPCONV_PLATFORM_UNKNOWN
#endif

// Compilers
#if defined(_MSC_VER)
#define SYMPCONV_COMPILER_VS
#elif defined(__clang__)
#define SYMPCONV_COMPILER_CLANG
#elif defined(__GNUC__)
#define SYMPCONV_COMPILER_GCC
#else
#define SYMPCONV_COMPILER_UNKNOWN
#endif

// SYMPCONV_FORCE_INLINE macro
#if defined(SYMPCONV_COMPILER_VS)
#define SYMPCONV_FORCE_INLINE __forceinline
#elif defined(SYMPCONV_COMPILER_GCC) || defined(SYMPCONV_COMPILER_CLANG)
#define SYMPCONV_FORCE_INLINE inline __attribute__((always_inline))
#else
#define SYMPCONV_FORCE_INLINE inline
#endif

#endif // SYMPCONV_ENVIROMENT_H
