# Project Jarvis Architecture

## Engineering direction

Jarvis is a cross-platform hybrid assistant with a native C++ core and optional Python integrations.

- C++20 is the baseline for performance-critical runtime components.
- Python remains available for ecosystem-heavy integrations and ML experiments.
- Local and cloud model providers will implement a shared inference interface.
- The tool runtime validates arguments and handles failures independently of the model.
- Model-generated tool requests are proposals, not authorization. Permission checks remain in the execution layer.

## First milestone: native tool engine

The initial native milestone provides:

- Typed tool arguments.
- A registry for named tools.
- Required-argument, unexpected-argument, and type validation.
- Consistent success/error results.
- Exception containment at the tool boundary.
- Built-in time, date, echo, and arithmetic tools.
- A small CLI and dependency-free tests.

This milestone intentionally does not include an LLM, JSON parser, shell execution, filesystem mutation, or Python bridge. Those will be added after the deterministic execution foundation is tested.

## Build

Requires CMake 3.20+ and a C++20 compiler.

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
    ctest --test-dir build --output-on-failure

On multi-configuration generators such as Visual Studio, use the Release configuration:

    cmake --build build --config Release
    ctest --test-dir build -C Release --output-on-failure

## Planned evolution

1. Harden and benchmark the native tool runtime.
2. Add permission policies and bounded execution.
3. Add a Python plugin adapter without making Python mandatory for native tools.
4. Add local inference through llama.cpp.
5. Add the Gemini provider behind the same model interface.
6. Add hybrid routing, memory, and user interfaces.
