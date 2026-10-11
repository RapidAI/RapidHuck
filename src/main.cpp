#include "game.h"

#include "raylib.h"

#include <string>

namespace {

void DrawCentered(const char* text, int centerX, int y, int fontSize, Color color) {
    const int w = MeasureText(text, fontSize);
    DrawText(text, centerX - w / 2, y, fontSize, color);
}

}  // namespace

int main() {
    const int cell = 40;
    const int cols = 20;
    const int rows = 15;
    const int hudHeight = 64;

    const int screenWidth = cols * cell;
    const int screenHeight = rows * cell + hudHeight;

    InitWindow(screenWidth, screenHeight, "RapidHuck - Snake");
    SetTargetFPS(60);

    rapidhuck::SnakeGame game(cols, rows);

    while (!WindowShouldClose()) {
        game.handleInput();
        game.update();

        BeginDrawing();
        ClearBackground(Color{15, 18, 25, 255});

        // --- HUD -------------------------------------------------------
        DrawRectangle(0, 0, screenWidth, hudHeight, Color{20, 24, 33, 255});
        DrawText("RapidHuck Snake", 16, 10, 26, Color{230, 235, 245, 255});

        const std::string scoreText = "Score: " + std::to_string(game.score());
        const std::string bestText = "Best: " + std::to_string(game.highScore());
        DrawText(scoreText.c_str(), 16, 40, 18, Color{104, 211, 145, 255});
        DrawText(bestText.c_str(), 140, 40, 18, Color{170, 175, 190, 255});

        DrawText("Arrows / WASD move   P pause   R restart   Esc quit",
                 screenWidth - 452, 24, 16, Color{150, 155, 170, 255});

        // --- Board -----------------------------------------------------
        game.draw(0, hudHeight, cell);

        // --- Overlays --------------------------------------------------
        const int cx = screenWidth / 2;
        const int cy = hudHeight + (rows * cell) / 2;

        if (game.state() == rapidhuck::State::Ready) {
            DrawRectangle(0, hudHeight, screenWidth, rows * cell, Color{0, 0, 0, 140});
            DrawCentered("SNAKE", cx, cy - 60, 56, Color{104, 211, 145, 255});
            DrawCentered("Press an arrow key / WASD / Enter to start", cx, cy + 10, 20,
                         Color{230, 235, 245, 255});
        } else if (game.state() == rapidhuck::State::Paused) {
            DrawRectangle(0, hudHeight, screenWidth, rows * cell, Color{0, 0, 0, 140});
            DrawCentered("PAUSED", cx, cy - 20, 48, Color{240, 200, 90, 255});
            DrawCentered("Press P to resume", cx, cy + 40, 20, Color{230, 235, 245, 255});
        } else if (game.state() == rapidhuck::State::GameOver) {
            DrawRectangle(0, hudHeight, screenWidth, rows * cell, Color{0, 0, 0, 170});
            DrawCentered("GAME OVER", cx, cy - 50, 52, Color{235, 87, 87, 255});
            const std::string finalScore = "Score: " + std::to_string(game.score());
            DrawCentered(finalScore.c_str(), cx, cy + 20, 24, Color{230, 235, 245, 255});
            DrawCentered("Press R to play again", cx, cy + 60, 20, Color{170, 175, 190, 255});
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
