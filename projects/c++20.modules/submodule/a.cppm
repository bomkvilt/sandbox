module;

#include <string>

export module test_module:a;


namespace NTestModule {
    export class TPublicClassA {
    public:
        std::string Name() const;
    };
}
