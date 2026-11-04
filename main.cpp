
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
	Vector3 origin = {0.0f, 0.0f, 0.0f};

    // Main loop
    while (!WindowShouldClose()){

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}	

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

				//x-axis
				DrawLine3D(origin, (Vector3){5.0f, 0.0f, 0.0f}, (Color)BLACK);				
				//y-axis
				DrawLine3D(origin, (Vector3){0.0f, 5.0f, 0.0f}, (Color)BLACK);
				//z-axis
				DrawLine3D(origin, (Vector3){0.0f, 0.0f, 5.0f}, (Color)BLACK);
				
				DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText("Mouse: orbit camera", 20, 55, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}