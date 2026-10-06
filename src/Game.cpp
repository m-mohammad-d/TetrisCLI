#include "Game.h"
#include <algorithm>
#include <thread>

Game::Game()
    : m_rng(std::random_device{}())
    , m_score(0)
    , m_lines(0)
    , m_level(1)
    , m_isPaused(false)
    , m_isGameOver(false)
    , m_isRunning(true)
    , m_dropTimer(0) {
    reset();
}

void Game::reset() {
    m_board.reset();
    m_bag.clear();
    m_score = 0;
    m_lines = 0;
    m_level = 1;
    m_isPaused = false;
    m_isGameOver = false;
    m_dropTimer = std::chrono::milliseconds(0);

    // Initial pieces
    m_currentPiece = Tetromino(drawNextPieceType());
    m_nextPiece = Tetromino(drawNextPieceType());
}

void Game::refillBag() {
    // Standard 7-bag randomizer system ensures fair distribution
    m_bag = {
        PieceType::I,
        PieceType::O,
        PieceType::T,
        PieceType::S,
        PieceType::Z,
        PieceType::J,
        PieceType::L
    };
    std::shuffle(m_bag.begin(), m_bag.end(), m_rng);
}

PieceType Game::drawNextPieceType() {
    if (m_bag.empty()) {
        refillBag();
    }
    PieceType next = m_bag.back();
    m_bag.pop_back();
    return next;
}

void Game::spawnPiece() {
    m_currentPiece = m_nextPiece;
    m_nextPiece = Tetromino(drawNextPieceType());

    // Check if spawn position is blocked -> Game Over
    if (!m_board.isValidPosition(m_currentPiece)) {
        m_isGameOver = true;
    }
}

bool Game::tryMove(int dx, int dy) {
    Point newPos = m_currentPiece.getPosition();
    newPos.x += dx;
    newPos.y += dy;

    if (m_board.isValidPosition(m_currentPiece, newPos, m_currentPiece.getRotation())) {
        m_currentPiece.setPosition(newPos);
        return true;
    }
    return false;
}

bool Game::tryRotateCW() {
    int oldRot = m_currentPiece.getRotation();
    int newRot = (oldRot + 1) % 4;

    // Test wall-kicks
    auto kicks = m_currentPiece.getWallKickOffsets(oldRot, newRot);
    Point basePos = m_currentPiece.getPosition();

    for (const auto& kick : kicks) {
        Point testPos = {basePos.x + kick.x, basePos.y + kick.y};
        if (m_board.isValidPosition(m_currentPiece, testPos, newRot)) {
            m_currentPiece.rotateClockwise();
            m_currentPiece.setPosition(testPos);
            return true;
        }
    }
    return false;
}

bool Game::tryRotateCCW() {
    int oldRot = m_currentPiece.getRotation();
    int newRot = (oldRot + 3) % 4;

    // Test wall-kicks
    auto kicks = m_currentPiece.getWallKickOffsets(oldRot, newRot);
    Point basePos = m_currentPiece.getPosition();

    for (const auto& kick : kicks) {
        Point testPos = {basePos.x + kick.x, basePos.y + kick.y};
        if (m_board.isValidPosition(m_currentPiece, testPos, newRot)) {
            m_currentPiece.rotateCounterClockwise();
            m_currentPiece.setPosition(testPos);
            return true;
        }
    }
    return false;
}

void Game::hardDrop() {
    int dropDistance = 0;
    while (tryMove(0, 1)) {
        dropDistance++;
    }
    // Hard drop bonus: 2 points per cell dropped
    m_score += dropDistance * 2;
    onPieceLocked();
}

Point Game::calculateGhostPosition() const {
    Point ghostPos = m_currentPiece.getPosition();
    while (m_board.isValidPosition(m_currentPiece, Point{ghostPos.x, ghostPos.y + 1}, m_currentPiece.getRotation())) {
        ghostPos.y += 1;
    }
    return ghostPos;
}

void Game::onPieceLocked() {
    m_board.lockPiece(m_currentPiece);
    int cleared = m_board.clearFullLines();
    if (cleared > 0) {
        addScoreForLines(cleared);
        m_lines += cleared;

        // Level increases every 10 cleared lines
        m_level = 1 + (m_lines / 10);
    }
    spawnPiece();
}

void Game::addScoreForLines(int count) {
    // Standard guideline scoring:
    // 1 line:  100 * level
    // 2 lines: 300 * level
    // 3 lines: 500 * level
    // 4 lines: 800 * level
    switch (count) {
        case 1:
            m_score += 100 * m_level;
            break;
        case 2:
            m_score += 300 * m_level;
            break;
        case 3:
            m_score += 500 * m_level;
            break;
        case 4:
            m_score += 800 * m_level;
            break;
        default:
            break;
    }
}

std::chrono::milliseconds Game::getDropInterval() const {
    // Formula decreasing drop interval as level rises, minimum 80ms
    int intervalMs = std::max(80, 800 - (m_level - 1) * 70);
    return std::chrono::milliseconds(intervalMs);
}

void Game::handleAction(Action action) {
    if (action == Action::Quit) {
        m_isRunning = false;
        return;
    }

    if (action == Action::Restart) {
        reset();
        return;
    }

    if (action == Action::Pause) {
        if (!m_isGameOver) {
            m_isPaused = !m_isPaused;
        }
        return;
    }

    // Actions below only apply when game is actively running
    if (m_isPaused || m_isGameOver) {
        return;
    }

    switch (action) {
        case Action::MoveLeft:
            tryMove(-1, 0);
            break;
        case Action::MoveRight:
            tryMove(1, 0);
            break;
        case Action::SoftDrop:
            if (tryMove(0, 1)) {
                m_score += 1; // Soft drop bonus: 1 point per cell
            }
            break;
        case Action::HardDrop:
            hardDrop();
            break;
        case Action::RotateCW:
            tryRotateCW();
            break;
        case Action::RotateCCW:
            tryRotateCCW();
            break;
        default:
            break;
    }
}

void Game::update(std::chrono::milliseconds dt) {
    if (m_isPaused || m_isGameOver) {
        return;
    }

    m_dropTimer += dt;
    if (m_dropTimer >= getDropInterval()) {
        m_dropTimer = std::chrono::milliseconds(0);
        if (!tryMove(0, 1)) {
            // Cannot drop further, lock piece
            onPieceLocked();
        }
    }
}

void Game::run() {
    using clock = std::chrono::steady_clock;
    auto lastTime = clock::now();

    while (m_isRunning) {
        auto currentTime = clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
        lastTime = currentTime;

        // 1. Process player input
        auto actions = m_input.pollActions();
        for (auto action : actions) {
            handleAction(action);
            if (!m_isRunning) break;
        }

        // 2. Update game simulation
        update(dt);

        // 3. Render state
        GameStateView view{
            m_board,
            m_currentPiece,
            m_nextPiece,
            calculateGhostPosition(),
            m_score,
            m_lines,
            m_level,
            m_isPaused,
            m_isGameOver
        };
        m_renderer.render(view);

        // Target ~60 FPS loop
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
