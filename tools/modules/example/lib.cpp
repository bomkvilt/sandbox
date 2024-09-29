module;

#include <sstream>
#include <string_view>

module hello;

auto say_hello(const std::string_view&name) -> std::string {
    auto ss = std::stringstream{} << "Hello " << name << "!\n";
    return ss.str();
}
