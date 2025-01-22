#if defined(_WIN32)  // Windows platform
    #if defined(BASE_BUILD_LIBTYPE_SHARED)
        #define BASEAPI __declspec(dllexport)
    #elif defined(BASE_USE_LIBTYPE_SHARED)
        #define BASEAPI __declspec(dllimport)
    #else
        #define BASEAPI
    #endif
#else  // Unix-like platforms
    #if defined(BASE_BUILD_LIBTYPE_SHARED)
        #define BASEAPI __attribute__((visibility("default")))
    #else
        #define BASEAPI
    #endif
#endif

#ifndef BASEAPI
    #define BASEAPI
#endif
