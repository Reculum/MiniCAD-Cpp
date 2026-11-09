
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "ArrayQueue.h"
#include <iostream>
#include <vector>


void MoveCameraEvent(Camera& camera){if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){UpdateCamera(&camera, CAMERA_FREE);}}

void CreatePointEvent(PointArrayQueue& ActivePoints){
	
	if (IsKeyPressed(KEY_P)){
				ControlPoint3D p = ControlPoint3D();
				p.ControlPointFromInput();
				ActivePoints.Append(p);
	}
}

void SelectPointEvent(PointArrayQueue& ActivePoints, Camera& camera){

	if ( (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))){
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		int clstIdx = -1;
		float minDist = INFINITY;

		for (int k = 0; k < ActivePoints.getSize(); k++){

			ControlPoint3D& tmpPoint = ActivePoints.getItem(k);

			//filter for visible points

			if (tmpPoint.IsVisible()){

				//get collision data

				RayCollision tmpCollision = GetRayCollisionSphere(mouseRay, tmpPoint.getCoords(), 0.05f);

				bool PointHit = tmpCollision.hit;

				//if we get a hit and the distance is smaller than the one previously recorded
				if (PointHit && (tmpCollision.distance < minDist)){ clstIdx = k; minDist = tmpCollision.distance;}
				
			}

		}

		//if we ever found a point

		if (clstIdx >= 0){
			ControlPoint3D& selectedPoint = ActivePoints.getItem(clstIdx);
			
			if (selectedPoint.IsSelected()){selectedPoint.Unselect();}
			else{selectedPoint.Select();}		
			
		}

	}

	

}

void RemoveSelectedPoints(PointArrayQueue& ActivePoints){

	if (IsKeyPressed(KEY_DELETE)){
		
		std::vector<int> indexesToBeDeleted = ActivePoints.FindSelectedPoints();

		for (int k = 0; k < indexesToBeDeleted.size(); k++){
			ActivePoints.RemoveItem(indexesToBeDeleted[k]);
		}
	}

}

ControlPoint3D ComputeMouseRayXZintersection(Ray MouseRay, ControlPoint3D p){

	Vector3 RaydDir = MouseRay.direction;
	Vector3 RayPos = MouseRay.position;
	Vector3 pCoords = p.getCoords();

	float pY = pCoords.y;
	float RayY = RayPos.y;
	float RayDirY =  RaydDir.y;

	if (fabsf(RayDirY) < 1e-6f){return p;};

	float t = (pY - RayY) / RayDirY;

	if (t < 0.0f){return p;};

	Vector3 interCoords = RayPos + t * RaydDir;

	float interX = interCoords.x;
	float interZ = interCoords.z;

	float interY = interCoords.y;

	return ControlPoint3D(interX, interZ, interY);
}


void MoveSelectedPoints(PointArrayQueue& ActivePoints, Camera& camera){

	if (IsKeyDown(KEY_M)){

		std::vector<int> indexesToBeMoved = ActivePoints.FindSelectedPoints();

		if (indexesToBeMoved.size() < 1){std::cout << "no points to be moved!!!" << std::endl; return;}
		if (indexesToBeMoved.size() >= 2){std::cout << "you are trying to move too many points!!!" << std::endl; return;}

		ControlPoint3D p = ActivePoints.getItem( indexesToBeMoved[0] );
		Vector2 mousePos = GetMousePosition();
		Ray mouseRay = GetScreenToWorldRay(mousePos, camera);

		ControlPoint3D newPos = ComputeMouseRayXZintersection(mouseRay, p);

		newPos.Plot();


	}


}