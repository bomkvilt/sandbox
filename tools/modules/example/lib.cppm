module;

#include <string>
#include <string_view>

export module hello;

export auto say_hello(const std::string_view &name) -> std::string;
