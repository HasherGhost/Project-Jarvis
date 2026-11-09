#include "jarvis/tool_registry.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace jarvis {
namespace {

std::tm local_time_now() {
    const auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
    std::tm local_time{};

#ifdef _WIN32
    if (localtime_s(&local_time, &now) != 0) {
        throw std::runtime_error("Unable to read local time.");
    }
#else
    if (localtime_r(&now, &local_time) == nullptr) {
        throw std::runtime_error("Unable to read local time.");
    }
#endif
    return local_time;
}

std::string format_time(const char* format) {
    const auto value = local_time_now();
    std::ostringstream output;
    output << std::put_time(&value, format);
    return output.str();
}

} // namespace

void register_builtin_tools(ToolRegistry& registry) {
    registry.register_tool({
        "get_time", "Get the current local time.", {},
        [](const ToolArguments&) {
            return ToolResult::ok(format_time("%H:%M:%S"));
        }
    });

    registry.register_tool({
        "get_date", "Get the current local date.", {},
        [](const ToolArguments&) {
            return ToolResult::ok(format_time("%Y-%m-%d"));
        }
    });

    registry.register_tool({
        "echo", "Return the supplied message.",
        {{"message", ArgumentType::string, true, "Message to return."}},
        [](const ToolArguments& arguments) {
            return ToolResult::ok(std::get<std::string>(arguments.at("message")));
        }
    });

    registry.register_tool({
        "add_numbers", "Add two numbers.",
        {
            {"a", ArgumentType::number, true, "First number."},
            {"b", ArgumentType::number, true, "Second number."}
        },
        [](const ToolArguments& arguments) {
            const auto a = std::get<double>(arguments.at("a"));
            const auto b = std::get<double>(arguments.at("b"));
            return ToolResult::ok(std::to_string(a + b));
        }
    });
}

} // namespace jarvis
