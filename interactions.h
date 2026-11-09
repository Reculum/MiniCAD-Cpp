#pragma once

#include "raylib.h"
#include "ArrayQueue.h"

void MoveCameraEvent(Camera& camera);
void CreatePointEvent(PointArrayQueue& ActivePoints);
void SelectPointEvent(PointArrayQueue& ActivePoints, Camera& camera);
void RemoveSelectedPoints(PointArrayQueue& ActivePoints);
void MoveSelectedPoints(PointArrayQueue& ActivePoints, Camera& camera);