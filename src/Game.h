#pragma once

#include "Board.h"
#include "Tetromino.h"
#include "InputHandler.h"
#include "Renderer.h"
#include <chrono>
#include <random>
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    void init();
    void reset();
    void update(std::chrono::milliseconds dt);
    void handleAction(Action action);

    // Tetromino spawning and bag generation
    void refillBag();
    PieceType drawNextPieceType();
    void spawnPiece();

    // Movement and rotation operations
    bool tryMove(int dx, int dy);
    bool tryRotateCW();
    bool tryRotateCCW();
    void hardDrop();

    // Ghost piece calculation
    Point calculateGhostPosition() const;

    // Line clearing and scoring
    void onPieceLocked();
    void addScoreForLines(int count);

    // Drop speed calculation based on level
    std::chrono::milliseconds getDropInterval() const;

private:
    Board m_board;
    Tetromino m_currentPiece;
    Tetromino m_nextPiece;

    InputHandler m_input;
    Renderer m_renderer;

    std::mt19937 m_rng;
    std::vector<PieceType> m_bag;

    int m_score;
    int m_lines;
    int m_level;

    bool m_isPaused;
    bool m_isGameOver;
    bool m_isRunning;

    std::chrono::milliseconds m_dropTimer;
};
