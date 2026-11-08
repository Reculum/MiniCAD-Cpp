#pragma once

#include "raylib.h"
#include "ArrayQueue.h"

void MoveCameraEvent(Camera& camera);
void CreatePointEvent(PointArrayQueue& ActivePoints);
void SelectPointEvent(PointArrayQueue& SelectedPoints, PointArrayQueue& ActivePoints, Camera& camera);