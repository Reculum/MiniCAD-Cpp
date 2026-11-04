
#include "raylib.h"

int main()
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "My first raylib application");

    SetTargetFPS(60);

    Vector2 circlePosition = {
        screenWidth / 2.0f,
        screenHeight / 2.0f
    };

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        if (IsKeyDown(KEY_RIGHT)) circlePosition.x += 3.0f;
        if (IsKeyDown(KEY_LEFT))  circlePosition.x -= 3.0f;
        if (IsKeyDown(KEY_UP))    circlePosition.y -= 3.0f;
        if (IsKeyDown(KEY_DOWN))  circlePosition.y += 3.0f;

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Hello, raylib!", 20, 20, 30, DARKGRAY);

            DrawCircleV(circlePosition, 30, BLUE);

            DrawFPS(10, screenHeight - 30);

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}