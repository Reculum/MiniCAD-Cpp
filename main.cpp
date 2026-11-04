
#include "raylib.h"
#include <cstring>

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

	// Create mesh
    Mesh mesh = { 0 };

    mesh.vertexCount = 4;
    mesh.triangleCount = 2;

    mesh.vertices = (float*) MemAlloc(12 * sizeof(float));

    mesh.indices = (unsigned short*) MemAlloc(6 * sizeof(unsigned short));

    float vertices[] = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0
    };

    unsigned short indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    memcpy(mesh.vertices, vertices, sizeof(vertices));
    memcpy(mesh.indices, indices, sizeof(indices));

    // Upload to GPU
    UploadMesh(&mesh, false);

    // Create model
    Model model = LoadModelFromMesh(mesh);

    // Main loop
    while (!WindowShouldClose()){

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}	

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

				DrawModel(model, origin, 1.0, (Color) BLUE);                

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