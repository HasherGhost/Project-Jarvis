#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace jarvis {

using ToolValue = std::variant<std::string, double, bool>;
using ToolArguments = std::unordered_map<std::string, ToolValue>;

enum class ArgumentType { string, number, boolean };

struct ParameterSpec {
    std::string name;
    ArgumentType type;
    bool required{true};
    std::string description;
};

struct ToolResult {
    bool success{false};
    std::string output;

    static ToolResult ok(std::string output);
    static ToolResult error(std::string message);
};

struct ToolDefinition {
    std::string name;
    std::string description;
    std::vector<ParameterSpec> parameters;
    std::function<ToolResult(const ToolArguments&)> handler;
};

class ToolRegistry {
public:
    void register_tool(ToolDefinition definition);
    ToolResult execute(const std::string& name, const ToolArguments& arguments) const;
    std::vector<ToolDefinition> list_tools() const;
    bool contains(const std::string& name) const noexcept;

private:
    static ToolResult validate_arguments(
        const ToolDefinition& definition,
        const ToolArguments& arguments
    );

    std::unordered_map<std::string, ToolDefinition> tools_;
};

void register_builtin_tools(ToolRegistry& registry);

} // namespace jarvis
