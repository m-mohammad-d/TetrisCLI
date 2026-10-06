#include "Board.h"

Board::Board() {
    reset();
}

void Board::reset() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            m_grid[y][x] = PieceType::None;
        }
    }
}

bool Board::isInside(int x, int y) const {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

bool Board::isCellEmpty(int x, int y) const {
    if (!isInside(x, y)) {
        return false;
    }
    return m_grid[y][x] == PieceType::None;
}

bool Board::isValidPosition(const Tetromino& piece, Point offset, int rotation) const {
    auto blocks = Tetromino::getLocalBlocks(piece.getType(), rotation);
    for (const auto& b : blocks) {
        int x = offset.x + b.x;
        int y = offset.y + b.y;

        // Check horizontal boundaries and bottom floor
        if (x < 0 || x >= WIDTH || y >= HEIGHT) {
            return false;
        }

        // Allow pieces above the ceiling (y < 0) as long as x is in bounds
        if (y < 0) {
            continue;
        }

        // Check collision with already placed blocks
        if (m_grid[y][x] != PieceType::None) {
            return false;
        }
    }
    return true;
}

bool Board::isValidPosition(const Tetromino& piece) const {
    return isValidPosition(piece, piece.getPosition(), piece.getRotation());
}

void Board::lockPiece(const Tetromino& piece) {
    auto blocks = piece.getBlockPositions();
    for (const auto& b : blocks) {
        if (isInside(b.x, b.y)) {
            m_grid[b.y][b.x] = piece.getType();
        }
    }
}

int Board::clearFullLines() {
    int clearedCount = 0;

    // Scan from bottom to top
    for (int y = HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (m_grid[y][x] == PieceType::None) {
                full = false;
                break;
            }
        }

        if (full) {
            clearedCount++;
            // Shift all rows above down by one
            for (int r = y; r > 0; --r) {
                m_grid[r] = m_grid[r - 1];
            }
            // Clear top row
            for (int x = 0; x < WIDTH; ++x) {
                m_grid[0][x] = PieceType::None;
            }
            // Re-check current row index since rows shifted down
            y++;
        }
    }

    return clearedCount;
}

PieceType Board::getCell(int x, int y) const {
    if (isInside(x, y)) {
        return m_grid[y][x];
    }
    return PieceType::None;
}

void Board::setCell(int x, int y, PieceType type) {
    if (isInside(x, y)) {
        m_grid[y][x] = type;
    }
}

