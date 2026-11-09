#include "jarvis/tool_registry.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace jarvis {
namespace {

bool matches_type(const ToolValue& value, ArgumentType expected) {
    switch (expected) {
        case ArgumentType::string:
            return std::holds_alternative<std::string>(value);
        case ArgumentType::number:
            return std::holds_alternative<double>(value) &&
                   std::isfinite(std::get<double>(value));
        case ArgumentType::boolean:
            return std::holds_alternative<bool>(value);
    }
    return false;
}

} // namespace

ToolResult ToolResult::ok(std::string output) {
    return {true, std::move(output)};
}

ToolResult ToolResult::error(std::string message) {
    return {false, std::move(message)};
}

void ToolRegistry::register_tool(ToolDefinition definition) {
    if (definition.name.empty()) {
        throw std::invalid_argument("Tool name cannot be empty.");
    }
    if (!definition.handler) {
        throw std::invalid_argument("Tool handler cannot be empty: " + definition.name);
    }

    for (std::size_t i = 0; i < definition.parameters.size(); ++i) {
        for (std::size_t j = i + 1; j < definition.parameters.size(); ++j) {
            if (definition.parameters[i].name == definition.parameters[j].name) {
                throw std::invalid_argument(
                    "Duplicate parameter '" + definition.parameters[i].name +
                    "' in tool '" + definition.name + "'."
                );
            }
        }
    }

    const auto name = definition.name;
    const auto [iterator, inserted] = tools_.emplace(name, std::move(definition));
    (void)iterator;
    if (!inserted) {
        throw std::invalid_argument("Tool is already registered: " + name);
    }
}

ToolResult ToolRegistry::validate_arguments(
    const ToolDefinition& definition,
    const ToolArguments& arguments
) {
    for (const auto& [argument_name, value] : arguments) {
        (void)value;
        bool known_argument = false;
        for (const auto& parameter : definition.parameters) {
            if (parameter.name == argument_name) {
                known_argument = true;
                break;
            }
        }
        if (!known_argument) {
            return ToolResult::error(
                "Unexpected argument '" + argument_name +
                "' for tool '" + definition.name + "'."
            );
        }
    }

    for (const auto& parameter : definition.parameters) {
        const auto argument = arguments.find(parameter.name);
        if (argument == arguments.end()) {
            if (parameter.required) {
                return ToolResult::error(
                    "Missing required argument '" + parameter.name +
                    "' for tool '" + definition.name + "'."
                );
            }
            continue;
        }
        if (!matches_type(argument->second, parameter.type)) {
            return ToolResult::error(
                "Argument '" + parameter.name + "' has the wrong type or a non-finite number."
            );
        }
    }
    return ToolResult::ok("Arguments are valid.");
}

ToolResult ToolRegistry::execute(
    const std::string& name,
    const ToolArguments& arguments
) const {
    const auto tool = tools_.find(name);
    if (tool == tools_.end()) {
        return ToolResult::error("Unknown tool: " + name);
    }

    const auto validation = validate_arguments(tool->second, arguments);
    if (!validation.success) {
        return validation;
    }

    try {
        return tool->second.handler(arguments);
    } catch (const std::exception& exception) {
        return ToolResult::error("Tool '" + name + "' failed: " + exception.what());
    } catch (...) {
        return ToolResult::error("Tool '" + name + "' failed with an unknown error.");
    }
}

std::vector<ToolDefinition> ToolRegistry::list_tools() const {
    std::vector<ToolDefinition> definitions;
    definitions.reserve(tools_.size());
    for (const auto& [name, definition] : tools_) {
        (void)name;
        definitions.push_back(definition);
    }
    return definitions;
}

bool ToolRegistry::contains(const std::string& name) const noexcept {
    return tools_.find(name) != tools_.end();
}

} // namespace jarvis
