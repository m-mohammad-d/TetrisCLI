#include "Tetromino.h"

Tetromino::Tetromino()
    : m_type(PieceType::None), m_rotation(0), m_position{3, 0} {}

Tetromino::Tetromino(PieceType type)
    : m_type(type), m_rotation(0), m_position{3, 0} {
    // For O piece, spawning at x = 4 centers it nicely on a 10-wide board
    if (m_type == PieceType::O) {
        m_position = {4, 0};
    }
}

std::array<Point, 4> Tetromino::getLocalBlocks(PieceType type, int rotation) {
    int rot = (rotation % 4 + 4) % 4;

    switch (type) {
        case PieceType::I: {
            // 4x4 bounding box
            // rot 0: horizontal in row 1
            // rot 1: vertical in col 2
            // rot 2: horizontal in row 2
            // rot 3: vertical in col 1
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{3, 1}},
                {Point{2, 0}, Point{2, 1}, Point{2, 2}, Point{2, 3}},
                {Point{0, 2}, Point{1, 2}, Point{2, 2}, Point{3, 2}},
                {Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{1, 3}}
            }};
            return shapes[rot];
        }
        case PieceType::O: {
            // 2x2 bounding box (rotation is invariant)
            return {Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{1, 1}};
        }
        case PieceType::T: {
            // 3x3 bounding box
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}}, // spawn: pointing up
                {Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}}, // pointing right
                {Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{1, 2}}, // pointing down
                {Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}}  // pointing left
            }};
            return shapes[rot];
        }
        case PieceType::S: {
            // 3x3 bounding box
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{1, 0}, Point{2, 0}, Point{0, 1}, Point{1, 1}},
                {Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
                {Point{1, 1}, Point{2, 1}, Point{0, 2}, Point{1, 2}},
                {Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}}
            }};
            return shapes[rot];
        }
        case PieceType::Z: {
            // 3x3 bounding box
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{2, 1}},
                {Point{2, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
                {Point{0, 1}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
                {Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2}}
            }};
            return shapes[rot];
        }
        case PieceType::J: {
            // 3x3 bounding box
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
                {Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{1, 2}},
                {Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
                {Point{1, 0}, Point{1, 1}, Point{0, 2}, Point{1, 2}}
            }};
            return shapes[rot];
        }
        case PieceType::L: {
            // 3x3 bounding box
            static constexpr std::array<std::array<Point, 4>, 4> shapes = {{
                {Point{2, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
                {Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
                {Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{0, 2}},
                {Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{1, 2}}
            }};
            return shapes[rot];
        }
        default:
            return {Point{0, 0}, Point{0, 0}, Point{0, 0}, Point{0, 0}};
    }
}

std::array<Point, 4> Tetromino::getLocalBlockPositions() const {
    return getLocalBlocks(m_type, m_rotation);
}

std::array<Point, 4> Tetromino::getBlockPositions() const {
    auto local = getLocalBlockPositions();
    for (auto& pt : local) {
        pt.x += m_position.x;
        pt.y += m_position.y;
    }
    return local;
}

void Tetromino::rotateClockwise() {
    m_rotation = (m_rotation + 1) % 4;
}

void Tetromino::rotateCounterClockwise() {
    m_rotation = (m_rotation + 3) % 4;
}

std::vector<Point> Tetromino::getWallKickOffsets(int fromRot, int toRot) const {
    if (m_type == PieceType::O) {
        return {{0, 0}};
    }

    // Standard Super Rotation System (SRS) wall kicks
    // (dx, dy) in board coordinates where +y is DOWNwards
    // SRS standard uses +y as UP, so dy tests are inverted: dy_board = -dy_srs.
    if (m_type == PieceType::I) {
        if (fromRot == 0 && toRot == 1) return {{0, 0}, {-2, 0}, { 1, 0}, {-2,  1}, { 1, -2}};
        if (fromRot == 1 && toRot == 0) return {{0, 0}, { 2, 0}, {-1, 0}, { 2, -1}, {-1,  2}};
        if (fromRot == 1 && toRot == 2) return {{0, 0}, {-1, 0}, { 2, 0}, {-1, -2}, { 2,  1}};
        if (fromRot == 2 && toRot == 1) return {{0, 0}, { 1, 0}, {-2, 0}, { 1,  2}, {-2, -1}};
        if (fromRot == 2 && toRot == 3) return {{0, 0}, { 2, 0}, {-1, 0}, { 2, -1}, {-1,  2}};
        if (fromRot == 3 && toRot == 2) return {{0, 0}, {-2, 0}, { 1, 0}, {-2,  1}, { 1, -2}};
        if (fromRot == 3 && toRot == 0) return {{0, 0}, { 1, 0}, {-2, 0}, { 1,  2}, {-2, -1}};
        if (fromRot == 0 && toRot == 3) return {{0, 0}, {-1, 0}, { 2, 0}, {-1, -2}, { 2,  1}};
    } else {
        // J, L, S, T, Z pieces
        if (fromRot == 0 && toRot == 1) return {{0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2}};
        if (fromRot == 1 && toRot == 0) return {{0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2}};
        if (fromRot == 1 && toRot == 2) return {{0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2}};
        if (fromRot == 2 && toRot == 1) return {{0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2}};
        if (fromRot == 2 && toRot == 3) return {{0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2}};
        if (fromRot == 3 && toRot == 2) return {{0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2}};
        if (fromRot == 3 && toRot == 0) return {{0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2}};
        if (fromRot == 0 && toRot == 3) return {{0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2}};
    }

    return {{0, 0}};
}

const char* Tetromino::getColorAnsi() const {
    return getColorAnsi(m_type);
}

const char* Tetromino::getColorAnsi(PieceType type) {
    switch (type) {
        case PieceType::I: return "\x1b[96m"; // Cyan
        case PieceType::O: return "\x1b[93m"; // Yellow
        case PieceType::T: return "\x1b[95m"; // Magenta
        case PieceType::S: return "\x1b[92m"; // Green
        case PieceType::Z: return "\x1b[91m"; // Red
        case PieceType::J: return "\x1b[94m"; // Blue
        case PieceType::L: return "\x1b[38;5;208m"; // Orange (or bright red/yellow)
        default:           return "\x1b[0m";  // Reset
    }
}

