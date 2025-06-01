#include "collector.hpp"

#include <ranges>
#include <regex>
#include <vector>

namespace error_codes {
    std::string ErrorCollector::format_report(this const Self& self) {
        std::ostringstream ss;

        auto multiline = [re = std::regex{R"(\n)"}](std::string& record) -> void {
            record = std::regex_replace(record, re, "\n  ");
        };

        {
            auto sink = [&, called = false] mutable -> std::ostream& {
                if (not called) {
                    called = true;
                    ss << "general errors:\n";
                }
                return ss;
            };
            for (const auto& [ec, ids] : self.errors_a) {
                for (const auto& id : ids) {
                    sink() << "- " << ec.message() << " a <" << id << ">\n";
                }
            }
            for (const auto& [ec, ids] : self.errors_b) {
                for (const auto& id : ids) {
                    sink() << "- " << ec.message() << " b <" << id << ">\n";
                }
            }
        }
        if (self.errors.size() > 0) {
            std::vector<std::string> records;
            records.reserve(self.errors.size());

            std::ranges::copy(self.errors, std::back_inserter(records));
            std::ranges::for_each(records, multiline);
            std::ranges::sort(records);

            ss << "detailed errors:\n";
            for (const auto& record : records) {
                ss << "- " << record << "\n";
            }
        }
        return std::move(ss).str();
    }
} // namespace error_codes
