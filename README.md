# Project Jarvis

A cross-platform hybrid AI assistant under active development.

## Current direction

- **Core:** C++20 for performance-critical runtime components.
- **Integrations:** Python remains optional for ML and ecosystem-heavy plugins.
- **Intelligence:** local and cloud providers behind a common interface.
- **Safety:** tool validation and permission checks are separate from model decisions.

## Native tool engine (first milestone)

The initial C++ engine includes a typed tool registry, argument validation, consistent results, and built-in time, date, echo, and arithmetic tools. It intentionally runs without an LLM so that tool execution can be tested deterministically.

### Requirements

- CMake 3.20 or newer
- A C++20 compiler (MSVC, GCC, or Clang)

### Build

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
    ctest --test-dir build --output-on-failure

For Visual Studio / other multi-configuration generators:

    cmake --build build --config Release
    ctest --test-dir build -C Release --output-on-failure

Run the executable produced by CMake. Available commands are: help, tools, time, date, echo <message>, add <number> <number>, and exit.

See docs/architecture.md for the development plan.

The existing Python prototype is retained during the gradual migration.
