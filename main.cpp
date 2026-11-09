
#include "raylib.h"

#include "interactions.h"
#include "ArrayQueue.h"

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

	Vector3 origin = (Vector3){0.0f, 0.0f, 0.0f};

    SetTargetFPS(60);

	PointArrayQueue ActivePoints = PointArrayQueue();
	

    // Main loop
    while (!WindowShouldClose()){

		MoveCameraEvent(camera);
		CreatePointEvent(ActivePoints);
		SelectPointEvent(ActivePoints, camera);
		RemoveSelectedPoints(ActivePoints);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

			//first, we draw the opaque objects

				//x-axis
				DrawLine3D(origin, (Vector3){5.0f, 0.0f, 0.0f}, (Color)RED);				
				//y-axis
				DrawLine3D(origin, (Vector3){0.0f, 5.0f, 0.0f}, (Color)BLUE);
				//z-axis
				DrawLine3D(origin, (Vector3){0.0f, 0.0f, 5.0f}, (Color)GREEN);

				//std-grid
				DrawGrid(10, 1.0f);

				//all the control points, so far
				ActivePoints.Plot();

			//then we can process everything that's transparent

				MoveSelectedPoints(ActivePoints, camera);	

            EndMode3D();

            DrawText("Mouse: orbit camera", 20, 55, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}