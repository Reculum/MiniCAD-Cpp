#pragma once

#include "raylib.h"
#include "ArrayQueue.h"
#include "curveGenerator.h"
#include "interactions.h"

void MoveCameraEvent(Camera& camera);
void CreatePointEvent(PointArrayQueue<ControlPoint3D>& ctivePoints);
void SelectPointEvent(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera);
void RemoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints);

void MoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera);
void CreateNurbsCurve(PointArrayQueue<ControlPoint3D>& ActivePoints, NURBS_Curve*& ActiveNurb, int degree);