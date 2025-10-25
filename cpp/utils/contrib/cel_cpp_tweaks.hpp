#pragma once

#include "cpp/utils/compiler/pragma.hpp"

// Compiler-agnostic warning suppression macros
#define SUPPRESS_CEL_CPP_WARNINGS_BEGIN()            \
    DIAGNOSTICS_PUSH()                               \
    DIAGNOSTICS_IGNORE("-Wdeprecated")               \
    DIAGNOSTICS_IGNORE("-Wnullability-completeness") \
    //

#define SUPPRESS_CEL_CPP_WARNINGS_END() \
    DIAGNOSTICS_POP()                   \
    //
