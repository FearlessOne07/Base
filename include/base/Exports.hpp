#if defined(_WIN32) || defined(__WIN32__)
    #ifdef BASE_EXPORT
        #define BASEAPI __declspec(dllexport)
    #else
        #define BASEAPI __declspec(dllimport)
    #endif
#else
    // For non-Windows platforms
    #define BASEAPI
#endif
