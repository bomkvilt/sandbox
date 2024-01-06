module;

#include <ranges>

module a_study;

namespace NA {
    std::string TA::Name() {
        return "abc";
    }

    std::string TA::A() {
        return std::string{"abcdefg"}
            | std::views::filter([&](const auto& v) { return v != 'a'; })
            | std::views::to<std::string>();
    }
}
