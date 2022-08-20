#ifndef CORVUS_SOURCE_CORVUS_CORE_PLATFORMDETECTION_H
#define CORVUS_SOURCE_CORVUS_CORE_PLATFORMDETECTION_H

// Defining platform related values

#ifdef _WIN32
    #ifdef _WIN64
        /* Windows x64 */
        #define CORVUS_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #error "x86 builds are not supported!"
    #endif
#else
    #error "Your platform currently not supported!"
#endif

#endif // !CORVUS_SOURCE_CORVUS_CORE_PLATFORMDETECTION_H