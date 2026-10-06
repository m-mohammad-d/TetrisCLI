# Tetris CLI

A modern, high-performance console Tetris game written in **C++17** for Windows terminals. Built with standard library components, Windows Console APIs, and ANSI escape sequences. Features zero external GUI/engine dependencies, flicker-free rendering, standard Tetris Super Rotation System (SRS) wall kicks, ghost piece preview, and standard 7-bag piece generation.

---

## Features

- **Standard Grid**: 10 columns × 20 rows board.
- **Full Tetromino Set**: All 7 standard pieces (`I`, `O`, `T`, `S`, `Z`, `J`, `L`) with standard color coding.
- **SRS Wall-Kick System**: Realistic rotation near walls and other placed blocks.
- **Fair Randomizer**: Standard 7-bag randomizer system (each tetromino appears once per 7-piece bag).
- **Ghost Piece**: Transparent projection indicator showing where the piece will land.
- **Double-Buffered Console Output**: Avoids screen-clearing flicker by buffering frames into a single write and using cursor repositioning (`\x1b[H`).
- **RAII Terminal Management**: Cursor is hidden during gameplay and automatically restored alongside terminal modes on exit or abnormal termination.
- **Progressive Difficulty**: Falling speed accelerates as cleared lines advance player level (every 10 lines).
- **Responsive Controls**: Non-blocking asynchronous input handling using Windows `_kbhit()` and `_getch()`.

---

## Controls

All controls are centralized in [`InputHandler.cpp`](file:///f:/project/TetrisCLI/src/InputHandler.cpp):

| Action | Primary Key | Secondary Key |
| :--- | :--- | :--- |
| **Move Left** | `A` | `Left Arrow` |
| **Move Right** | `D` | `Right Arrow` |
| **Soft Drop** | `S` | `Down Arrow` |
| **Hard Drop** | `Space` | |
| **Rotate Clockwise** | `W` | `Up Arrow` |
| **Rotate Counter-Clockwise** | `Q` | |
| **Pause / Resume** | `P` | |
| **Restart Game** | `R` | |
| **Quit Game** | `Esc` | |

---

## Scoring System

- **1 Line Cleared**: `100 × Level`
- **2 Lines Cleared**: `300 × Level`
- **3 Lines Cleared**: `500 × Level`
- **4 Lines Cleared (Tetris!)**: `800 × Level`
- **Soft Drop**: `+1` point per cell dropped
- **Hard Drop**: `+2` points per cell dropped

---

## Project Structure

```
TetrisCLI/
├── CMakeLists.txt          # Build configuration (C++17)
├── README.md               # Documentation and build instructions
└── src/
    ├── main.cpp            # Application entry point & exception wrapper
    ├── Game.h / .cpp       # Core game loop, tick timing, bag randomizer, scoring
    ├── Board.h / .cpp      # 10x20 grid state, collision checks, line clearing
    ├── Tetromino.h / .cpp  # Piece matrices, SRS wall-kick offset tables, colors
    ├── Renderer.h / .cpp   # Double-buffered ANSI renderer, cursor & console management
    └── InputHandler.h / .cpp # Non-blocking Windows console input polling
```

---

## Architecture Overview

1. **[`Game`](file:///f:/project/TetrisCLI/src/Game.h)**:
   - Owns the main loop (~60 FPS), tracks delta time, and coordinates falling ticks.
   - Manages the 7-bag random generator for fair piece selection.
   - Handles game state transitions: Active, Paused, Game Over, and Restart.
2. **[`Board`](file:///f:/project/TetrisCLI/src/Board.h)**:
   - Represents the 10×20 playing field.
   - Performs boundary and block collision checks for piece positioning and rotations.
   - Sweeps and clears full rows, shifting remaining rows downward.
3. **[`Tetromino`](file:///f:/project/TetrisCLI/src/Tetromino.h)**:
   - Stores bounding boxes for the 4 rotation states of each piece.
   - Contains SRS wall-kick offset transition tables.
4. **[`Renderer`](file:///f:/project/TetrisCLI/src/Renderer.h)**:
   - Sets console code pages to UTF-8 and enables Windows Virtual Terminal Processing.
   - Hides the console cursor on start and restores console state using RAII on destruct.
   - Compiles each frame into an in-memory buffer before dispatching a single flush, eliminating flicker.
5. **[`InputHandler`](file:///f:/project/TetrisCLI/src/InputHandler.h)**:
   - Polls `_kbhit()` and `_getch()` non-blockingly.
   - Decodes multi-byte escape sequences (such as Windows arrow keys) into clean `Action` enums.

---

## Requirements

- **Operating System**: Windows 10 / 11 (with Windows Terminal, PowerShell, or cmd.exe)
- **Compiler**: Any C++17 compatible compiler:
  - GCC / MinGW-w64 (`g++` >= 8.0)
  - Clang (`clang++` >= 7.0)
  - MSVC (Visual Studio 2019 / 2022)
- **Build System**: CMake 3.15+ (and Ninja or MSBuild / MinGW Makefiles)

---

## Build Instructions

### Option 1: Using CMake with Ninja / MinGW

Open PowerShell in the project directory:

```powershell
cmake -B build -G "Ninja"
cmake --build build
```

*(If using MSVC Developer Command Prompt, you can also run `cmake -B build` followed by `cmake --build build --config Release`).*

### Option 2: Direct compilation with g++ or cl.exe

If CMake is not available, compile all sources directly:

**Using g++ (MinGW / UCRT64):**
```powershell
g++ -std=c++17 -O2 -I src src/*.cpp -o tetris.exe
```

**Using MSVC (Developer Command Prompt):**
```cmd
cl /std:c++17 /O2 /I src src\*.cpp /Fe:tetris.exe
```

---

## Running the Game

Launch the generated executable directly in your terminal:

```powershell
.\build\tetris.exe
```
