# System & Build Requirements

This document outlines the software, compiler, and tooling requirements necessary to build, run, and develop the Tetris CLI project.

---

## 1. Operating System

- **Target OS**: Windows 10 (version 1809 or newer) or Windows 11.
- **Terminal Emulator**:
  - Windows Terminal (Recommended for standard ANSI 256-color support and smooth UTF-8 rendering)
  - PowerShell 5.1+ / PowerShell Core (pwsh 7+)
  - Command Prompt (`cmd.exe`)

---

## 2. Compiler Requirements

The codebase uses **C++17** language features (`std::array`, `std::clamp`/`std::max`, structured bindings, `constexpr`, and scoped enums). Any of the following compilers or newer will work:

- **GCC (MinGW-w64 / MSYS2 / UCRT64)**: GCC 8.0 or newer (tested with GCC 16.2.0)
- **Clang (LLVM)**: Clang 7.0 or newer
- **Microsoft Visual C++ (MSVC)**: Visual Studio 2019 (version 16.0) or Visual Studio 2022 (MSVC toolset v142/v143 or newer)

---

## 3. Build Tools

At least one build toolchain is required:

### Primary Recommended Toolchain:
- **CMake**: Version **3.15** or newer
- **Generator**:
  - **Ninja** (version 1.10+) OR
  - **MinGW Makefiles** (via `mingw32-make`) OR
  - **Visual Studio Solution Generator** (`Visual Studio 16 2019` / `Visual Studio 17 2022`)

### Standalone Alternative (No CMake needed):
- Direct compilation with `g++` (MinGW) or `cl.exe` (MSVC Developer Command Prompt).

---

## 4. Dependencies & Libraries

- **External Libraries**: **None**.
  - No SDL, SFML, ncurses, PDCurses, or external game engines.
- **C++ Standard Library**: Standard C++17 library (`<vector>`, `<array>`, `<chrono>`, `<random>`, `<algorithm>`, `<thread>`, `<iostream>`, `<sstream>`).
- **Operating System APIs**:
  - Windows Console API (`<windows.h>`, `<conio.h>`)
  - Virtual Terminal Processing (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`)
  - UTF-8 Code Page support (`SetConsoleOutputCP(CP_UTF8)`)

---

## 5. Summary Matrix

| Component | Minimum Version | Recommended Version |
| :--- | :--- | :--- |
| **Windows** | Windows 10 (1809) | Windows 11 / Windows Terminal |
| **C++ Standard** | C++17 | C++17 or C++20 |
| **CMake** | 3.15 | 3.25+ |
| **Compiler (GCC)** | 8.1 | 13.0+ / UCRT64 |
| **Compiler (MSVC)**| VS 2019 (v142) | VS 2022 (v143) |
| **Dependencies** | None (Zero 3rd-party) | None |
