
#include "raylib.h"

int main()
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "MiniCAD - NURBS Visualizer");

    // Camera
    Camera3D camera = { 0 };

    camera.position = { 5.0f, 5.0f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()){
		
        // Update
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Reference grid
                DrawGrid(20, 1.0f);

                // Coordinate axes
                DrawLine3D(
                    { 0, 0, 0 },
                    { 3, 0, 0 },
                    RED
                );

                DrawLine3D(
                    { 0, 0, 0 },
                    { 0, 3, 0 },
                    GREEN
                );

                DrawLine3D(
                    { 0, 0, 0 },
                    { 0, 0, 3 },
                    BLUE
                );

                // Reference object
                DrawCube(
                    { 0, 0.5f, 0 },
                    1.0f, 1.0f, 1.0f,
                    SKYBLUE
                );

                DrawCubeWires(
                    { 0, 0.5f, 0 },
                    1.0f, 1.0f, 1.0f,
                    DARKBLUE
                );

            EndMode3D();

            DrawText("3D NURBS Viewer", 20, 20, 25, DARKGRAY);
            DrawText("Mouse: orbit camera", 20, 55, 18, GRAY);
            DrawFPS(20, screenHeight - 30);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}