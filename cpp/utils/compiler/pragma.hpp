#pragma once

/// @note [cpp.pragma.op]
#define PRAGMA(x) _Pragma(#x)

/// Diagnostics Control Pragmas
#ifdef __clang__
    #define DIAGNOSTICS_PUSH() PRAGMA(clang diagnostic push)
    #define DIAGNOSTICS_POP() PRAGMA(clang diagnostic pop)
    #define DIAGNOSTICS_IGNORE(ARG) PRAGMA(clang diagnostic ignored ARG)
#elif defined(__GNUC__)
    #define DIAGNOSTICS_PUSH() PRAGMA(GCC diagnostic push)
    #define DIAGNOSTICS_POP() PRAGMA(GCC diagnostic pop)
    #define DIAGNOSTICS_IGNORE(ARG) PRAGMA(GCC diagnostic ignored ARG)
#else
    #error "unsupported compiler"
#endif
