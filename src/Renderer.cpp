#include "Renderer.h"
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

Renderer::Renderer() {
    initConsole();
    hideCursor();
}

Renderer::~Renderer() {
    showCursor();
    restoreConsole();
}

void Renderer::initConsole() {
#ifdef _WIN32
    // Set console output code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        GetConsoleMode(hOut, &m_originalOutMode);
        // Enable virtual terminal processing for ANSI escape sequences
        DWORD mode = m_originalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, mode);
    }

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn != INVALID_HANDLE_VALUE) {
        GetConsoleMode(hIn, &m_originalInMode);
        // Disable mouse and window input events so getch doesn't pick up junk
        DWORD mode = m_originalInMode & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
        SetConsoleMode(hIn, mode);
    }
#endif
    // Clear screen once at start
    std::cout << "\x1b[2J" << std::flush;
}

void Renderer::restoreConsole() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE && m_originalOutMode != 0) {
        SetConsoleMode(hOut, m_originalOutMode);
    }
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn != INVALID_HANDLE_VALUE && m_originalInMode != 0) {
        SetConsoleMode(hIn, m_originalInMode);
    }
#endif
    // Reset colors and ensure cursor is on a fresh line
    std::cout << "\x1b[0m\n" << std::flush;
}

void Renderer::hideCursor() {
    std::cout << "\x1b[?25l" << std::flush;
}

void Renderer::showCursor() {
    std::cout << "\x1b[?25h" << std::flush;
}

void Renderer::render(const GameStateView& state) {
    m_frameBuffer.clear();
    // Reserve reasonable buffer size to prevent reallocations
    m_frameBuffer.reserve(4096);

    // Reposition cursor to top-left (home position) instead of clearing screen to prevent flicker
    m_frameBuffer.append("\x1b[H");

    // Pre-calculate current piece blocks and ghost piece blocks
    auto currentBlocks = state.currentPiece.getBlockPositions();
    Tetromino ghostPiece = state.currentPiece;
    ghostPiece.setPosition(state.ghostPiecePos);
    auto ghostBlocks = ghostPiece.getBlockPositions();

    // Next piece local blocks
    auto nextBlocks = state.nextPiece.getLocalBlockPositions();

    // Header title
    m_frameBuffer.append("  \x1b[1;36m=============================\x1b[0m\n");
    m_frameBuffer.append("  \x1b[1;33m       T E T R I S           \x1b[0m\n");
    m_frameBuffer.append("  \x1b[1;36m=============================\x1b[0m\n\n");

    // Top border of board
    m_frameBuffer.append("  \x1b[90m+");
    for (int x = 0; x < Board::WIDTH; ++x) {
        m_frameBuffer.append("--");
    }
    m_frameBuffer.append("+\x1b[0m");

    // Sidebar line 0
    m_frameBuffer.append("    \x1b[1;37mSCORE:\x1b[0m  \x1b[92m");
    m_frameBuffer.append(std::to_string(state.score));
    m_frameBuffer.append("\x1b[0m\n");

    // Board rows: 0 to HEIGHT - 1
    for (int y = 0; y < Board::HEIGHT; ++y) {
        m_frameBuffer.append("  \x1b[90m|\x1b[0m");

        for (int x = 0; x < Board::WIDTH; ++x) {
            bool isCurrent = false;
            for (const auto& b : currentBlocks) {
                if (b.x == x && b.y == y) {
                    isCurrent = true;
                    break;
                }
            }

            bool isGhost = false;
            if (!isCurrent) {
                for (const auto& b : ghostBlocks) {
                    if (b.x == x && b.y == y) {
                        isGhost = true;
                        break;
                    }
                }
            }

            if (isCurrent) {
                m_frameBuffer.append(state.currentPiece.getColorAnsi());
                m_frameBuffer.append("[]\x1b[0m");
            } else if (isGhost) {
                // Ghost piece in faint outline / dimmed color
                m_frameBuffer.append("\x1b[90m::\x1b[0m");
            } else {
                PieceType cell = state.board.getCell(x, y);
                if (cell != PieceType::None) {
                    m_frameBuffer.append(Tetromino::getColorAnsi(cell));
                    m_frameBuffer.append("[]\x1b[0m");
                } else {
                    // Empty cell
                    m_frameBuffer.append("  ");
                }
            }
        }

        m_frameBuffer.append("\x1b[90m|\x1b[0m");

        // Sidebar information
        switch (y) {
            case 0:
                m_frameBuffer.append("    \x1b[1;37mLINES:\x1b[0m  \x1b[93m");
                m_frameBuffer.append(std::to_string(state.lines));
                m_frameBuffer.append("\x1b[0m");
                break;
            case 1:
                m_frameBuffer.append("    \x1b[1;37mLEVEL:\x1b[0m  \x1b[95m");
                m_frameBuffer.append(std::to_string(state.level));
                m_frameBuffer.append("\x1b[0m");
                break;
            case 3:
                m_frameBuffer.append("    \x1b[1;37mNEXT:\x1b[0m");
                break;
            case 4:
                m_frameBuffer.append("    \x1b[90m+--------+\x1b[0m");
                break;
            case 5:
            case 6:
            case 7:
            case 8: {
                int py = y - 5;
                m_frameBuffer.append("    \x1b[90m|\x1b[0m");
                for (int px = 0; px < 4; ++px) {
                    bool occupied = false;
                    for (const auto& nb : nextBlocks) {
                        if (nb.x == px && nb.y == py) {
                            occupied = true;
                            break;
                        }
                    }
                    if (occupied) {
                        m_frameBuffer.append(state.nextPiece.getColorAnsi());
                        m_frameBuffer.append("[]\x1b[0m");
                    } else {
                        m_frameBuffer.append("  ");
                    }
                }
                m_frameBuffer.append("\x1b[90m|\x1b[0m");
                break;
            }
            case 9:
                m_frameBuffer.append("    \x1b[90m+--------+\x1b[0m");
                break;
            case 12:
                m_frameBuffer.append("    \x1b[90mCONTROLS:\x1b[0m");
                break;
            case 13:
                m_frameBuffer.append("    A / Left  : Move Left");
                break;
            case 14:
                m_frameBuffer.append("    D / Right : Move Right");
                break;
            case 15:
                m_frameBuffer.append("    S / Down  : Soft Drop");
                break;
            case 16:
                m_frameBuffer.append("    W / Up    : Rotate CW");
                break;
            case 17:
                m_frameBuffer.append("    Q         : Rotate CCW");
                break;
            case 18:
                m_frameBuffer.append("    Space     : Hard Drop");
                break;
            case 19:
                m_frameBuffer.append("    P: Pause  R: Restart  Esc: Quit");
                break;
            default:
                break;
        }

        m_frameBuffer.append("\n");
    }

    // Bottom border of board
    m_frameBuffer.append("  \x1b[90m+");
    for (int x = 0; x < Board::WIDTH; ++x) {
        m_frameBuffer.append("--");
    }
    m_frameBuffer.append("+\x1b[0m\n");

    // Status line or banner
    if (state.isGameOver) {
        m_frameBuffer.append("\n  \x1b[1;91m*** GAME OVER! ***\x1b[0m  Press \x1b[1;93m[R]\x1b[0m to Restart or \x1b[1;91m[Esc]\x1b[0m to Quit.\x1b[K\n");
    } else if (state.isPaused) {
        m_frameBuffer.append("\n  \x1b[1;93m*** PAUSED ***\x1b[0m      Press \x1b[1;93m[P]\x1b[0m to Resume or \x1b[1;91m[Esc]\x1b[0m to Quit.\x1b[K\n");
    } else {
        m_frameBuffer.append("\n  \x1b[32mActive Game\x1b[0m                                                     \x1b[K\n");
    }

    // Flush entire frame buffer in one single write to eliminate flickering
    std::cout << m_frameBuffer << std::flush;
}
