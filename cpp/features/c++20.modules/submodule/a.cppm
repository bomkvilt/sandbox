module;

#include <string>

export module test_module:a;

namespace NTestModule {
    export class TPublicClassA {
    public:
        std::string Name() const;
    };
}

// Seems like module partitions cannot define private fragments (?)
// ```
// error: private module fragment declaration with no preceding module declaration
// ```

// module :private;
// module test_module:private;

// namespace NTestModule {
//     class TPrivateClassA {
//     public:
//         std::string Name() const;
//     };
// }
