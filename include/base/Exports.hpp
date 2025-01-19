#if defined(_WIN32)  // Windows platform
    #if defined(BASE_BUILD_LIBTYPE_SHARED)
        #define BASEAPI __declspec(dllexport)  // Export symbols when building a shared library
    #elif defined(BASE_USE_LIBTYPE_SHARED)
        #define BASEAPI __declspec(dllimport)  // Import symbols when using a shared library
    #else
        #define BASEAPI  // Empty for static libraries or unspecified cases
    #endif
#else  // Unix-like platforms
    #if defined(BASE_BUILD_LIBTYPE_SHARED)
        #define BASEAPI __attribute__((visibility("default")))  // Export symbols when building a shared library
    #else
        #define BASEAPI  // Empty for static libraries or unspecified cases
    #endif
#endif

#ifndef BASEAPI
    #define BASEAPI  // Fallback: Ensure BASEAPI is always defined (e.g., for static libraries)
#endif
