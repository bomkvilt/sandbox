module;

#include <string>
#include <ranges>


export module parac_compiler;


export class TA final {
public:
    std::string Name() {
        return "abc";
    }

    void A() {
        std::string a = "abcdefg";
        std::ranges::filter_view(a, [&](const auto& v) {
            return v != 'a';
        });
    }
};
