module;

#include <string>

export module test_module;

export import :a;

namespace NTestModule {
    export class TPublicClass {
    public:
        std::string Name() const;
    };
}

module :private;

namespace NTestModule {
    class TPrivateClass {
    public:
        std::string Name() const;
    };
}
