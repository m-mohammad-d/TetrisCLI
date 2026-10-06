#pragma once

#include <vector>
#include <array>
#include <string>

enum class PieceType {
    None = 0,
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Tetromino {
public:
    Tetromino();
    explicit Tetromino(PieceType type);

    PieceType getType() const { return m_type; }
    int getRotation() const { return m_rotation; }
    Point getPosition() const { return m_position; }
    void setPosition(const Point& pos) { m_position = pos; }

    // Returns the 4 block coordinates in board space
    std::array<Point, 4> getBlockPositions() const;

    // Returns the 4 block coordinates in local space (centered in bounding box)
    std::array<Point, 4> getLocalBlockPositions() const;

    // Returns local blocks for a given rotation state [0..3]
    static std::array<Point, 4> getLocalBlocks(PieceType type, int rotation);

    void rotateClockwise();
    void rotateCounterClockwise();

    // Wall kick offsets for tests: from current rotation to target rotation
    // Returns list of (dx, dy) translation tests
    std::vector<Point> getWallKickOffsets(int fromRot, int toRot) const;

    // Color code / ANSI styling tag
    const char* getColorAnsi() const;
    static const char* getColorAnsi(PieceType type);

private:
    PieceType m_type;
    int m_rotation; // 0, 1, 2, 3
    Point m_position; // top-left offset on board
};
