#pragma once

#include "Board.h"
#include "Tetromino.h"
#include <string>

struct GameStateView {
    const Board& board;
    const Tetromino& currentPiece;
    const Tetromino& nextPiece;
    Point ghostPiecePos;
    int score;
    int lines;
    int level;
    bool isPaused;
    bool isGameOver;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Prevent copy/move to maintain console state lifecycle
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void render(const GameStateView& state);

private:
    void initConsole();
    void restoreConsole();
    void hideCursor();
    void showCursor();

#ifdef _WIN32
    unsigned long m_originalOutMode = 0;
    unsigned long m_originalInMode = 0;
#endif

    std::string m_frameBuffer;
};

