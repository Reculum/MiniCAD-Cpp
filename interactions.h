#pragma once

#include "raylib.h"
#include "ArrayQueue.h"
#include "curveGenerator.h"
#include "surfaceGenerator.h"
#include "interactions.h"

void MoveCameraEvent(Camera& camera);

void CreatePointEvent(PointArrayQueue<ControlPoint3D>& ctivePoints);
void SelectPointEvent(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera);


void MoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints, Camera& camera);
void RemoveSelectedPoints(PointArrayQueue<ControlPoint3D>& ActivePoints);


void CreateNurbsCurve(PointArrayQueue<NURBS_Curve>& ActiveNurbs, PointArrayQueue<ControlPoint3D>& ActivePoints, int degree);
void SelectCurveEvent(PointArrayQueue<NURBS_Curve>& ActiveNurbs, Camera& camera);
void SelectSurfaceEvent(PointArrayQueue<NURBS_Surface>& ActiveSurfaces, Camera& camera);


void ChangePointWeightOnCurve(PointArrayQueue<NURBS_Curve>& ActiveNurbs);
void ChangePointWeightOnSurface(PointArrayQueue<NURBS_Surface>& ActiveSurface);


void RemoveSelectedCurves(PointArrayQueue<NURBS_Curve>& ActiveNurbs);
void RemoveSelectedSurfaces(PointArrayQueue<NURBS_Surface>& ActiveSurfaces);


void CreateNurbsSurface(PointArrayQueue<NURBS_Surface>& ActiveSurfaces, PointArrayQueue<ControlPoint3D>& ActivePoints, int numberOfControlPointsU, int numberOfControlPointsV, int p, int q);