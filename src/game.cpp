#include "game.h"

#include "raylib.h"

#include <algorithm>

namespace rapidhuck {

SnakeGame::SnakeGame(int cols, int rows)
    : cols_(cols), rows_(rows), rng_(std::random_device{}()) {
    reset();
}

void SnakeGame::reset() {
    snake_.clear();
    const int startX = cols_ / 2;
    const int startY = rows_ / 2;
    snake_.push_back({startX, startY});
    snake_.push_back({startX - 1, startY});
    snake_.push_back({startX - 2, startY});
    dir_ = {1, 0};
    nextDir_ = {1, 0};
    score_ = 0;
    frameCounter_ = 0;
    state_ = State::Ready;
    spawnFood();
}

bool SnakeGame::occupied(const Cell& c) const {
    return std::find(snake_.begin(), snake_.end(), c) != snake_.end();
}

void SnakeGame::spawnFood() {
    std::uniform_int_distribution<int> dx(0, cols_ - 1);
    std::uniform_int_distribution<int> dy(0, rows_ - 1);
    Cell c{dx(rng_), dy(rng_)};
    int guard = 0;
    while (occupied(c) && guard++ < cols_ * rows_ * 4) {
        c = {dx(rng_), dy(rng_)};
    }
    food_ = c;
}

void SnakeGame::handleInput() {
    if (state_ == State::GameOver) {
        if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
            reset();
        }
        return;
    }

    if (IsKeyPressed(KEY_P)) {
        if (state_ == State::Running) {
            state_ = State::Paused;
        } else if (state_ == State::Paused) {
            state_ = State::Running;
        }
        return;
    }

    Cell d = nextDir_;
    bool pressed = false;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        d = {0, -1};
        pressed = true;
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        d = {0, 1};
        pressed = true;
    } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        d = {-1, 0};
        pressed = true;
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        d = {1, 0};
        pressed = true;
    }

    // Reject a 180 degree turn into the current direction.
    if (pressed && !(d.x == -dir_.x && d.y == -dir_.y)) {
        nextDir_ = d;
    }

    if (state_ == State::Ready && (pressed || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
        state_ = State::Running;
    }
}

void SnakeGame::update() {
    if (state_ != State::Running) {
        return;
    }
    if (++frameCounter_ < framesPerMove_) {
        return;
    }
    frameCounter_ = 0;

    dir_ = nextDir_;
    const Cell head = snake_.front();
    const Cell next{head.x + dir_.x, head.y + dir_.y};

    // Wall collision.
    if (next.x < 0 || next.x >= cols_ || next.y < 0 || next.y >= rows_) {
        state_ = State::GameOver;
        highScore_ = std::max(highScore_, score_);
        return;
    }

    const bool eating = (next == food_);

    // Self collision. The tail cell frees up on the same tick unless we grow.
    const std::size_t limit = snake_.size();
    for (std::size_t i = 0; i < limit; ++i) {
        if (i == limit - 1 && !eating) {
            continue;
        }
        if (snake_[i] == next) {
            state_ = State::GameOver;
            highScore_ = std::max(highScore_, score_);
            return;
        }
    }

    snake_.push_front(next);
    if (eating) {
        score_ += 10;
        highScore_ = std::max(highScore_, score_);
        spawnFood();
    } else {
        snake_.pop_back();
    }
}

void SnakeGame::draw(int originX, int originY, int cell) const {
    // Checkerboard board.
    for (int y = 0; y < rows_; ++y) {
        for (int x = 0; x < cols_; ++x) {
            const Color c = ((x + y) % 2 == 0) ? Color{26, 32, 44, 255}
                                               : Color{21, 26, 36, 255};
            DrawRectangle(originX + x * cell, originY + y * cell, cell, cell, c);
        }
    }

    // Food.
    const float pad = cell * 0.16f;
    DrawRectangleRounded(
        Rectangle{static_cast<float>(originX + food_.x * cell) + pad,
                  static_cast<float>(originY + food_.y * cell) + pad,
                  static_cast<float>(cell) - 2.0f * pad,
                  static_cast<float>(cell) - 2.0f * pad},
        0.5f, 8, Color{235, 87, 87, 255});

    // Snake body (head drawn brighter).
    bool isHead = true;
    for (const Cell& s : snake_) {
        const Color c = isHead ? Color{104, 211, 145, 255} : Color{56, 161, 105, 255};
        DrawRectangleRounded(
            Rectangle{static_cast<float>(originX + s.x * cell) + 1.0f,
                      static_cast<float>(originY + s.y * cell) + 1.0f,
                      static_cast<float>(cell) - 2.0f,
                      static_cast<float>(cell) - 2.0f},
            0.35f, 6, c);
        isHead = false;
    }
}

}  // namespace rapidhuck
