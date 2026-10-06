#include "InputHandler.h"

#ifdef _WIN32
#include <conio.h>
#endif

std::vector<Action> InputHandler::pollActions() {
    std::vector<Action> actions;

#ifdef _WIN32
    while (_kbhit()) {
        int ch = _getch();

        // Handle Windows special key sequences (arrows, function keys)
        if (ch == 0 || ch == 224) {
            int code = _getch();
            switch (code) {
                case 72: // Up Arrow
                    actions.push_back(Action::RotateCW);
                    break;
                case 80: // Down Arrow
                    actions.push_back(Action::SoftDrop);
                    break;
                case 75: // Left Arrow
                    actions.push_back(Action::MoveLeft);
                    break;
                case 77: // Right Arrow
                    actions.push_back(Action::MoveRight);
                    break;
                default:
                    break;
            }
        } else {
            // Standard ASCII keys (case-insensitive for letters)
            switch (ch) {
                // Movement
                case 'a':
                case 'A':
                    actions.push_back(Action::MoveLeft);
                    break;
                case 'd':
                case 'D':
                    actions.push_back(Action::MoveRight);
                    break;
                case 's':
                case 'S':
                    actions.push_back(Action::SoftDrop);
                    break;

                // Rotation
                case 'w':
                case 'W':
                    actions.push_back(Action::RotateCW);
                    break;
                case 'q':
                case 'Q':
                    actions.push_back(Action::RotateCCW);
                    break;

                // Hard Drop
                case ' ':
                    actions.push_back(Action::HardDrop);
                    break;

                // Game Control
                case 'p':
                case 'P':
                    actions.push_back(Action::Pause);
                    break;
                case 'r':
                case 'R':
                    actions.push_back(Action::Restart);
                    break;
                case 27: // Escape key
                    actions.push_back(Action::Quit);
                    break;

                default:
                    break;
            }
        }
    }
#endif

    return actions;
}

