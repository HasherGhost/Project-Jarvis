#include "jarvis/tool_registry.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

} // namespace

int main() {
    try {
        jarvis::ToolRegistry registry;
        jarvis::register_builtin_tools(registry);

        require(registry.contains("get_time"), "get_time should be registered");
        require(registry.contains("get_date"), "get_date should be registered");
        require(registry.contains("echo"), "echo should be registered");

        const auto echo = registry.execute("echo", {{"message", std::string("hello")}});
        require(echo.success && echo.output == "hello", "echo should return its message");

        const auto sum = registry.execute("add_numbers", {{"a", 3.0}, {"b", 5.0}});
        require(sum.success && sum.output == "8", "add_numbers should format whole numbers cleanly");

        require(!registry.execute("echo", {}).success, "missing arguments should fail");
        require(
            !registry.execute("echo", {{"message", 42.0}}).success,
            "incorrect argument types should fail"
        );
        require(
            !registry.execute("add_numbers", {
                {"a", std::numeric_limits<double>::infinity()},
                {"b", 1.0}
            }).success,
            "non-finite numeric arguments should fail"
        );
        require(
            !registry.execute("get_time", {{"unexpected", true}}).success,
            "unexpected arguments should fail"
        );
        require(
            !registry.execute("does_not_exist", {}).success,
            "unknown tools should fail"
        );

        bool duplicate_rejected = false;
        try {
            registry.register_tool({
                "echo", "Duplicate tool.", {},
                [](const jarvis::ToolArguments&) {
                    return jarvis::ToolResult::ok("duplicate");
                }
            });
        } catch (const std::invalid_argument&) {
            duplicate_rejected = true;
        }
        require(duplicate_rejected, "duplicate registration should fail");

        require(registry.execute("get_time", {}).success, "get_time should return a value");
        require(registry.execute("get_date", {}).success, "get_date should return a value");

        std::cout << "All native tool registry tests passed.\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Test failure: " << exception.what() << '\n';
        return 1;
    }
}
