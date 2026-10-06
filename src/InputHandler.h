#pragma once

#include <vector>

enum class Action {
    None,
    MoveLeft,
    MoveRight,
    SoftDrop,
    HardDrop,
    RotateCW,
    RotateCCW,
    Pause,
    Restart,
    Quit
};

class InputHandler {
public:
    InputHandler() = default;

    // Poll keyboard non-blockingly and return any triggered game actions
    std::vector<Action> pollActions();
};

