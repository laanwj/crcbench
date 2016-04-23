#undef PLATFORM_IS_LITTLE_ENDIAN
#if defined(OS_MACOSX)
  #include <machine/endian.h>
  #if defined(__DARWIN_LITTLE_ENDIAN) && defined(__DARWIN_BYTE_ORDER)
    #define PLATFORM_IS_LITTLE_ENDIAN \
        (__DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN)
  #endif
#elif defined(OS_SOLARIS)
  #include <sys/isa_defs.h>
  #ifdef _LITTLE_ENDIAN
    #define PLATFORM_IS_LITTLE_ENDIAN true
  #else
    #define PLATFORM_IS_LITTLE_ENDIAN false
  #endif
#elif defined(OS_FREEBSD) || defined(OS_OPENBSD) ||\
      defined(OS_NETBSD) || defined(OS_DRAGONFLYBSD)
  #include <sys/types.h>
  #include <sys/endian.h>
  #define PLATFORM_IS_LITTLE_ENDIAN (_BYTE_ORDER == _LITTLE_ENDIAN)
#elif defined(OS_HPUX)
  #define PLATFORM_IS_LITTLE_ENDIAN false
#elif defined(OS_ANDROID)
  // Due to a bug in the NDK x86 <sys/endian.h> definition,
  // _BYTE_ORDER must be used instead of __BYTE_ORDER on Android.
  // See http://code.google.com/p/android/issues/detail?id=39824
  #include <endian.h>
  #define PLATFORM_IS_LITTLE_ENDIAN  (_BYTE_ORDER == _LITTLE_ENDIAN)
#else
  #include <endian.h>
#endif

#ifndef PLATFORM_IS_LITTLE_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#endif

inline uint32_t LE_LOAD32(const void* ptr) {
#ifdef PLATFORM_IS_LITTLE_ENDIAN
    // Load the raw bytes
    uint32_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
#else
    const uint8_t *ptr_ = static_cast<uint8_t*>(ptr);
    return ((static_cast<uint32_t>(ptr[0]))
        | (static_cast<uint32_t>(ptr[1]) << 8)
        | (static_cast<uint32_t>(ptr[2]) << 16)
        | (static_cast<uint32_t>(ptr[3]) << 24));
#endif
}

inline uint64_t LE_LOAD64(const void* ptr) {
#ifdef PLATFORM_IS_LITTLE_ENDIAN
    // Load the raw bytes
    uint64_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
#else
    const uint8_t *ptr_ = static_cast<uint8_t*>(ptr);
    return ((static_cast<uint64_t>(ptr[0]))
        | (static_cast<uint64_t>(ptr[1]) << 8)
        | (static_cast<uint64_t>(ptr[2]) << 16)
        | (static_cast<uint64_t>(ptr[3]) << 24)
        | (static_cast<uint64_t>(ptr[4]) << 32)
        | (static_cast<uint64_t>(ptr[5]) << 40)
        | (static_cast<uint64_t>(ptr[6]) << 48)
        | (static_cast<uint64_t>(ptr[7]) << 56));
#endif
}

