#include "jarvis/tool_registry.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void print_help() {
    std::cout
        << "\nJarvis native tool engine (early development build)\n"
        << "Commands:\n"
        << "  tools                 List registered tools\n"
        << "  time                  Get local time\n"
        << "  date                  Get local date\n"
        << "  echo <message>        Echo a message\n"
        << "  add <number> <number> Add two numbers\n"
        << "  help                  Show this help\n"
        << "  exit                  Quit Jarvis\n\n";
}

bool parse_number(const std::string& text, double& value) {
    try {
        std::size_t parsed = 0;
        value = std::stod(text, &parsed);
        return parsed == text.size() && std::isfinite(value);
    } catch (...) {
        return false;
    }
}

void print_result(const jarvis::ToolResult& result) {
    std::cout << (result.success ? "Jarvis: " : "Error: ")
              << result.output << '\n';
}

} // namespace

int main() {
    jarvis::ToolRegistry registry;
    jarvis::register_builtin_tools(registry);

    std::cout << "Jarvis native engine online. Type 'help' for commands.\n";
    std::string line;

    while (true) {
        std::cout << "\nYou: ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "exit" || line == "quit") {
            std::cout << "Jarvis: Goodbye.\n";
            break;
        } else if (line == "help") {
            print_help();
        } else if (line == "tools") {
            for (const auto& tool : registry.list_tools()) {
                std::cout << "  " << tool.name << " - " << tool.description << '\n';
            }
        } else if (line == "time") {
            print_result(registry.execute("get_time", {}));
        } else if (line == "date") {
            print_result(registry.execute("get_date", {}));
        } else if (line.rfind("echo ", 0) == 0) {
            print_result(registry.execute("echo", {{"message", line.substr(5)}}));
        } else if (line.rfind("add ", 0) == 0) {
            std::istringstream input(line.substr(4));
            std::string first, second, extra;
            double a = 0.0, b = 0.0;

            if (!(input >> first >> second) || (input >> extra) ||
                !parse_number(first, a) || !parse_number(second, b)) {
                std::cout << "Error: Usage: add <number> <number>\n";
                continue;
            }
            print_result(registry.execute("add_numbers", {{"a", a}, {"b", b}}));
        } else if (line.empty()) {
            continue;
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    return 0;
}
