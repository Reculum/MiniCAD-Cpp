
#include "raylib.h"
#include "raymath.h"

int main()
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "MiniCAD - NURBS Visualizer");

    // Camera
    Camera3D camera = { 0 };

    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

	Vector3 cubeCenter = {0.0f, 0.0f, 0.0f};
	Vector3 offset = {1.0f/60, 1.0f/60, 1.0f/60};

    // Main loop
    while (!WindowShouldClose()){

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Reference object
                DrawCube(
                    cubeCenter,
                    1.0f, 1.0f, 1.0f,
                    SKYBLUE
                );

                DrawCubeWires(
                    cubeCenter,
                    1.0f, 1.0f, 1.0f,
                    DARKBLUE
                );

				DrawGrid(10, 1.0f);

            EndMode3D();

			cubeCenter = Vector3Add(cubeCenter, offset);

            DrawText("3D NURBS Viewer", 20, 20, 25, DARKGRAY);
            DrawText("Mouse: orbit camera", 20, 55, 18, GRAY);
            DrawFPS(20, screenHeight - 30);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}