module;

#include <string>

// NOTE: `test_module:a` causes compilation errors
// ```
// CMake Error: Output <...>/submodule/a.cpp.o provides the `test_module:a` module but it is not found in a `FILE_SET` of type `CXX_MODULES`
// ```
module test_module;


namespace  NTestModule {
    std::string TPublicClassA::Name() const {
        return "public_class_a";
    }
}
