#pragma once

#include "Tetromino.h"
#include <array>
#include <vector>

class Board {
public:
    static constexpr int WIDTH = 10;
    static constexpr int HEIGHT = 20;

    Board();

    void reset();

    // Check if cell is within board boundaries
    bool isInside(int x, int y) const;

    // Check if cell is empty (vacant)
    bool isCellEmpty(int x, int y) const;

    // Check if a tetromino at a specific position and rotation is valid
    bool isValidPosition(const Tetromino& piece, Point offset, int rotation) const;
    bool isValidPosition(const Tetromino& piece) const;

    // Place a tetromino permanently onto the board
    void lockPiece(const Tetromino& piece);

    // Clear completed lines and return the number of cleared lines
    int clearFullLines();

    // Board cell access
    PieceType getCell(int x, int y) const;
    void setCell(int x, int y, PieceType type);

private:
    std::array<std::array<PieceType, WIDTH>, HEIGHT> m_grid;
};

