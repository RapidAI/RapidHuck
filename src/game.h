#pragma once

#include <cstddef>
#include <deque>
#include <random>

namespace rapidhuck {

// A single grid cell.
struct Cell {
    int x = 0;
    int y = 0;
    bool operator==(const Cell& other) const { return x == other.x && y == other.y; }
};

// High level game states.
enum class State { Ready, Running, Paused, GameOver };

// Pure game logic + rendering for the Snake game.
// It has no dependency on window creation so it can be unit tested / reused.
class SnakeGame {
public:
    SnakeGame(int cols, int rows);

    void reset();
    void handleInput();
    void update();
    void draw(int originX, int originY, int cell) const;

    State state() const { return state_; }
    int score() const { return score_; }
    int highScore() const { return highScore_; }
    int cols() const { return cols_; }
    int rows() const { return rows_; }

private:
    void spawnFood();
    bool occupied(const Cell& c) const;

    int cols_;
    int rows_;
    std::deque<Cell> snake_;
    Cell food_{};
    Cell dir_{1, 0};
    Cell nextDir_{1, 0};
    State state_ = State::Ready;
    int score_ = 0;
    int highScore_ = 0;
    int framesPerMove_ = 8;   // speed: game ticks every N rendered frames
    int frameCounter_ = 0;
    std::mt19937 rng_;
};

}  // namespace rapidhuck
